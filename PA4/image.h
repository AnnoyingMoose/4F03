/*
 * image.h
 * -------
 *
 * Contains definitions for the pixel and bm_image types.
 *
 * Written by:
 *   Emmanuel Thiessen (thiessen@mcmaster.ca, MSID#000945053)
 *   James Lee            (leejr@mcmaster.ca, MSID#001318125)
 *
 * for SFWR ENG 4F03 PA4 (Winter 2017)
 */

#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

typedef uint_least32_t pixel;

typedef struct
{
	int width;
	int height;
	pixel *pixels;
} bm_image;

#endif
