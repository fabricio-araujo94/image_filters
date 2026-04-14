#ifndef FILTERS_H
#define FILTERS_H

#include "image.h"

void filter_negative(Image *img);
void filter_brightness(Image *img, int amount);
void filter_contrast(Image *img, float factor);
void filter_mean_blur(Image *img);
void filter_sobel(Image *img);

#endif
