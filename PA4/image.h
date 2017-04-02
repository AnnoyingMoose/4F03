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

#define pixel_at(im, x, y) im->pixels[x * im->height + y]

#define chan_r(p) ((p & 0x00FF0000) >> 16)
#define chan_g(p) ((p & 0x0000FF00) >>  8)
#define chan_b(p) ((p & 0x000000FF) >>  0)

#define chan_r_at(im, x, y) chan_r(pixel_at(im, x, y))
#define chan_g_at(im, x, y) chan_g(pixel_at(im, x, y))
#define chan_b_at(im, x, y) chan_b(pixel_at(im, x, y))

#define buildPixel(r, g, b) (((pixel)r << 16) | ((pixel)g << 8) | (pixel)b)

#endif
