/*
 * blur.cu
 * ------
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

#include <stdlib.h>
#include <stdio.h>
#include <cuda.h>

__global__ void blurOnePixel(Image *srcImage, Image *dstImage, int rad)
{
	/*
	int myIDx = blockIdx.x;
	int myIDy = blockIdx.y;
	printf("Hello world from x:%i, y:%i\n", myIDx, myIDy);
	*/

	int x = blockIdx.x;
	int y = blockIdx.y;
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
	if (xmax >= srcImage->width)  xmax = srcImage->width  - 1;
	if (ymax >= srcImage->height) ymax = srcImage->height - 1;

	blurAreaSize = (xmax - xmin + 1) * (ymax - ymin + 1);

	for (j = ymin; j <= ymax; j++)
	for (i = xmin; i <= xmax; i++)
	for (k = 0; k < 3; k++)
	{
		pixel[k] += GetPixel(srcImage, i, j, k);
	}

	for (k = 0; k < 3; k++)
		SetPixel(dstImage, x, y, k, (unsigned char)(pixel[k] / blurAreaSize));
}


__host__ __device__ void
SetPixel(Image *image, int x, int y, int chan, unsigned char val)
{
	int offset = (y * image->width + x) * 3 + chan;

	image->data[offset] = val;
}


__host__ __device__ unsigned  char
GetPixel(Image *image, int x, int y, int chan)
{
	int offset = (y * image->width + x) * 3 + chan;

	return image->data[offset];
}


void blurPixel(Image *srcImage, Image *dstImage, int rad, int x, int y)
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
	if (xmax >= srcImage->width)  xmax = srcImage->width  - 1;
	if (ymax >= srcImage->height) ymax = srcImage->height - 1;

	blurAreaSize = (xmax - xmin + 1) * (ymax - ymin + 1);

	for (j = ymin; j <= ymax; j++)
	for (i = xmin; i <= xmax; i++)
	for (k = 0; k < 3; k++)
	{
		pixel[k] += ImageGetPixel(srcImage, i, j, k);
	}

	for (k = 0; k < 3; k++)
		ImageSetPixel(dstImage, x, y, k, (unsigned char)(pixel[k] / blurAreaSize));
}

void blurImage(Image *srcImage, Image *dstImage, int rad)
{
	/*
	int i, j;

	for (j = 0; j < srcImage->height; j++)
	for (i = 0; i < srcImage->width;  i++)
	{
		blurPixel(srcImage, dstImage, rad, i, j);
	}
	*/

	dim3 block(1, 1);
	dim3 grid(srcImage->height, srcImage->width);
	blurOnePixel<<<grid, block>>>(srcImage, dstImage, rad);
	cudaDeviceSynchronize();
}
