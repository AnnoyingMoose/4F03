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

pixel blurPixel(bm_image *image, int x, int y, int rad)
{
	unsigned long int
		r = 0, g = 0, b = 0;
	int
		i, j,
		xmin = x - rad,
		xmax = x + rad,
		ymin = y - rad,
		ymax = y + rad,
		blurAreaSize;

	if (xmin < 0) xmin = 0;
	if (ymin < 0) ymin = 0;
	if (xmax >= image->width)  xmax = image->width  - 1;
	if (ymax >= image->height) ymax = image->height - 1;

	blurAreaSize = (xmax - xmin) * (ymax - ymin);

	for (i = xmin; i <= xmax; i++)
	for (j = ymin; j <= ymax; j++)
	{
		r += chan_r_at(image, i, j);
		g += chan_g_at(image, i, j);
		b += chan_b_at(image, i, j);
	}

	r /= blurAreaSize;
	g /= blurAreaSize;
	b /= blurAreaSize;

	return buildPixel(r, g, b);
}

void blurImage(bm_image *srcImage, bm_image *dstImage, int rad)
{
	int i, j;

	for (i = 0; i < srcImage->width;  i++)
	for (j = 0; j < srcImage->height; j++)
	{
		pixel_at(dstImage, i, j) = blurPixel(srcImage, i, j, rad);
	}
}
