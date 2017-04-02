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

/**
 * Generates a square-blur-filtered version of the pixel at the given co-ordinates in a bitmap image.
 *
 * @param image The bitmap image
 * @param x     The x co-ordinate of the pixel
 * @param y     The y co-ordinate of the pixel
 * @param rad   The radius of the blur filter
 *
 * @returns The processed pixel
 */
pixel blurPixel(bm_image *image, int x, int y, int rad);

/**
 * Filters an entire bitmap image using a square-blur (low-pass) filter.
 *
 * @param srcImage The image to process
 * @param dstImage The storage location for the processed image (must have the same dimensions as srcImage)
 * @param rad      The radius of the blur filter
 */
void blurImage(bm_image *srcImage, bm_image *dstImage, int rad);

#endif
