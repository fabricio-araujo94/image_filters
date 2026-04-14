#include "../include/filters.h"

static inline uint8_t clamp(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return (uint8_t) value;
}

void filter_negative(Image *img) {
    if (!img || !img->data) return;

    int size = img->width * img->height * img->channels;
    uint8_t *p = img->data;

    for (int i = 0; i < size; i++) {
        *(p + i) = 255 - *(p + i);
    }
}

void filter_brightness(Image *img, int amount) {
    if (!img || !img->data) return;
    int size = img->width * img->height * img->channels;
    uint8_t *p = img->data;

    for (int i = 0; i < size; i++) {
        int new_val = *(p + i) + amount;
        *(p + i) = clamp(new_val);
    }
}

void filter_contrast(Image *img, float factor) {
    if (!img || !img->data) return;
    int size = img->width * img->height * img->channels;
    uint8_t *p = img->data;

    for (int i = 0; i < size; i++) {
        int new_val = (int) ((*(p + i) - 128) * factor + 128);
        *(p + i) = clamp(new_val);
    }
}
