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

#include "blur.h"

#include <assert.h>
#include <cuda.h>

__device__ void blurPixel(int rad, int width, int height, char *src, char *dst, int x, int y)
{
	unsigned long int
		pixel[3] = {0, 0, 0};
	int
		i, j, k,
		xmin = x - rad,
		xmax = x + rad,
		ymin = y - rad,
		ymax = y + rad,
		blurAreaSize;

	if (xmin < 0) xmin = 0;
	if (ymin < 0) ymin = 0;
	if (xmax >= width)  xmax = width  - 1;
	if (ymax >= height) ymax = height - 1;

	blurAreaSize = (xmax - xmin + 1) * (ymax - ymin + 1);

	for (j = ymin; j <= ymax; j++)
	for (i = xmin; i <= xmax; i++)
	for (k = 0; k < 3; k++)
	{
		//~ pixel[k] += ImageGetPixel(srcImage, i, j, k);
		pixel[k] += src[j * width * 3 + i * 3 + k];
	}

	for (k = 0; k < 3; k++)
		//~ ImageSetPixel(dstImage, x, y, k, (unsigned char)(pixel[k] / blurAreaSize));
		dst[j * width * 3 + i * 3 + k] = (unsigned char)(pixel[k] / blurAreaSize);
}

__global__ void gpuBlurImage(int rad, int width, int height, char *src, char *dst)
{
	int i, j, xmin, xmax, ymin, ymax;

	// Flatten grid/block co-ordinates
	tid =

	for (j = 0; j < height; j++)
	for (i = 0; i < width;  i++)
	{
		blurPixel(rad, width, height, src, dst, i, j);
	}
}

void blurImage(Image *srcImage, Image *dstImage, int rad)
{
	int
		width  = srcImage->width,
		height = srcImage->height;

	assert(width == dstImage->width && height == dstImage->height);

	dim3 b(32, 32, 1);
	dim3 g(ceil(width / 32.0), ceil(height / 32.0), 1);

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
