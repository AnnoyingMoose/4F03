/*
 * pa5.c
 * -----
 *
 * Written by:
 *   Emmanuel Thiessen (thiessen@mcmaster.ca, MSID#000945053)
 *   James Lee            (leejr@mcmaster.ca, MSID#001318125)
 *
 * for SFWR ENG 4F03 PA5 (Winter 2017)
 */

#include <stdlib.h>
#include <stdio.h>

#include "ppmFile.h"
#include "blur.h"

int main(int argc, char **argv)
{
	if (argc < 4)
	{
		fprintf(stderr, "Usage: %s <radius> <sourceImage> <destImage>\n", argv[0]);
		exit(1);
	}

	int radius = atoi(argv[1]);
	Image *srcImage, *dstImage;

	srcImage = ImageRead(argv[2]);
	dstImage = ImageCreate(srcImage->width, srcImage->height);

	blurImage(srcImage, dstImage, radius);

	ImageWrite(dstImage, argv[3]);

	free(srcImage->data);
	free(srcImage);
	free(dstImage->data);
	free(dstImage);

	return 0;
}
