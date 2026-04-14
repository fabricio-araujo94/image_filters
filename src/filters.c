#include "../include/filters.h"

void filter_negative(Image *img) {
    if (!img || !img->data) return;

    int size = img->width * img->height * img->channels;
    uint8_t *p = img->data;

    for (int i = 0; i < size; i++) {
        *(p + i) = 255 - *(p + i);
    }
}
