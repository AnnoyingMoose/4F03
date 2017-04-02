/*
 * blur.h
 * ------
 *
 * Interface declarations for the blurPixel and blurImage functions.
 *
 * Written by:
 *   Emmanuel Thiessen (thiessen@mcmaster.ca, MSID#000945053)
 *   James Lee            (leejr@mcmaster.ca, MSID#001318125)
 *
 * for SFWR ENG 4F03 PA4 (Winter 2017)
 */

#ifndef BLUR_H
#define BLUR_H

#include "image.h"

pixel blurPixel(bm_image *image, int x, int y, int rad);
void blurImage(bm_image *srcImage, bm_image *dstImage, int rad);

#endif
