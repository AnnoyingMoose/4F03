/*
 * blur.c
 * ------
 *
 * Definitions for the blurPixel and blurImage functions.
 *
 * Written by:
 *   Emmanuel Thiessen (thiessen@mcmaster.ca, MSID#000945053)
 *   James Lee            (leejr@mcmaster.ca, MSID#001318125)
 *
 * for SFWR ENG 4F03 PA4 (Winter 2017)
 */

#include "blur.h"

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
	int i, j;

	for (j = 0; j < srcImage->height; j++)
	for (i = 0; i < srcImage->width;  i++)
	{
		blurPixel(srcImage, dstImage, rad, i, j);
	}
}
