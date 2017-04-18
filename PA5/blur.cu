/*
 * blur.cu
 * -------
 *
 * Definitions for the blurPixel and blurImage functions.
 *
 * Written by:
 *   Emmanuel Thiessen (thiessen@mcmaster.ca, MSID#000945053)
 *   James Lee            (leejr@mcmaster.ca, MSID#001318125)
 *
 * for SFWR ENG 4F03 PA5 (Winter 2017)
 */

extern "C"
{
#include "blur.h"
}

#include <assert.h>
#include <cuda.h>

__device__ void blurPixel(int rad, int width, int height, char *src, char *dst, int x, int y)
{
	int
		i, j,
		xmin = x - rad,
		xmax = x + rad,
		ymin = y - rad,
		ymax = y + rad,
		blurAreaSize;

	double red = 0;
	double green = 0;
	double blue = 0;
	int fields_count = 0;

	if (xmin < 0) xmin = 0;
	if (ymin < 0) ymin = 0;
	if (xmax >= width)  xmax = width  - 1;
	if (ymax >= height) ymax = height - 1;

	blurAreaSize = (xmax - xmin + 1) * (ymax - ymin + 1);

	for (j = ymin; j <= ymax; j++)
	for (i = xmin; i <= xmax; i++)
	{
		int currentOffset = (x + i + j * blurAreaSize)*3;
		red += src[currentOffset];
		green += src[currentOffset+1];
		blue += src[currentOffset+2];
		fields_count++;
	}

	dst[x*3] = red/fields_count;
	dst[x*3+1] = green/fields_count;
	dst[x*3+2] = blue/fields_count;
}

__global__ void gpuBlurImage(int rad, int width, int height, char *src, char *dst)
{
	int
		x = blockIdx.x * blockDim.x + threadIdx.x,
		y = blockIdx.y * blockDim.y + threadIdx.y;

	if (x < width && y < height)
		blurPixel(rad, width, height, src, dst, x, y);
}

extern "C"
{
	void blurImage(Image *srcImage, Image *dstImage, int rad)
	{
		int
			width  = srcImage->width,
			height = srcImage->height;

		assert(width == dstImage->width && height == dstImage->height);

		dim3 b(1, 1);
		dim3 g(width, height);

		size_t sz = width * height * 3;
		char *src, *dst;

		cudaMalloc(&src, sz);
		cudaMalloc(&dst, sz);
		cudaMemcpy(src, srcImage->data, sz, cudaMemcpyHostToDevice);

		gpuBlurImage<<<g, b>>>(rad, width, height, src, dst);
		cudaDeviceSynchronize();

		cudaMemcpy(dstImage->data, dst, sz, cudaMemcpyDeviceToHost);
		cudaFree(src);
		cudaFree(dst);
	}
}
