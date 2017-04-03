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

#include "ppmFile.h"
#include "ppm-ext.h"

/**
 * Generates a square-blur-filtered version of the pixel at the given co-ordinates in a bitmap image.
 *
 * @param srcImage The source (original) image
 * @param dstImage The destination image (where the blurred pixel should be stored)
 * @param rad   The radius of the blur filter
 * @param x     The x co-ordinate of the pixel
 * @param y     The y co-ordinate of the pixel
 */
void blurPixel(Image *srcImage, Image *dstImage, int rad, int x, int y);

/**
 * Filters an entire bitmap image using a square-blur (low-pass) filter.
 *
 * @param srcImage The source (original) image
 * @param dstImage The destination image
 * @param rad   The radius of the blur filter
 */
void blurImage(Image *srcImage, Image *dstImage, int rad);

#endif
