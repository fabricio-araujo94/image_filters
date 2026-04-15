#include "../include/filters.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

static inline uint8_t clamp(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return (uint8_t) value;
}

static void sort_window(uint8_t window[], int size) {
    for (int i = 1; i < size; i++) {
        uint8_t key = window[i];
        int j = i - 1;

        while (j >= 0 && window[j] > key) {
            window[j + 1] = window[j];
            j--;
        }

        window[j + 1] = key;


    }
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


void filter_mean_blur(Image *img, int kernel_size) {
    if (!img || !img->data || kernel_size < 3) return;

    if (kernel_size % 2 == 0) kernel_size++;

    int w = img->width;
    int h = img->height;
    int channels = img->channels;
    int size = w * h * channels;
    int offset = kernel_size / 2;
    float num_pixels = (float) (kernel_size * kernel_size);

    uint8_t *temp = (uint8_t*) malloc(size);
    if (!temp) return;

    memcpy(temp, img->data, size);

    for (int y = offset; y < h - offset; y++) {
        for (int x = offset; x < w - offset; x++) {

            for (int c = 0; c < channels; c++) {
                long sum = 0;

                for (int ky = -offset; ky <= offset; ky++) {
                    for (int kx = -offset; kx <= offset; kx++) {
                        int idx = ((y + ky) * w + (x + kx)) * channels + c;
                        sum += temp[idx];
                    }
                }

                int current_idx = (y * w + x) * channels + c;
                img->data[current_idx] = (uint8_t) (sum / num_pixels);
            }
        }
    }

    free(temp);
}

void filter_separable_blur(Image *img, int kernel_size) {
    if (!img || !img->data || kernel_size < 3) return;

    if (kernel_size % 2 == 0) kernel_size++;

    int w = img->width;
    int h = img->height;
    int channels = img->channels;
    int size = w * h * channels;
    int offset = kernel_size / 2;

    uint8_t *temp = (uint8_t*) malloc(size);
    if (!temp) return;

    memcpy(temp, img->data, size);

    for (int y = offset; y < h - offset; y++) {
        for (int x = offset; x < w - offset; x++) {
            for(int c = 0; c < channels; c++) {

                long sum = 0;

                for (int kx = -offset; kx <= offset; kx++) {
                    int neighbor_idx = (y * w + (x + kx)) * channels + c;
                    sum += *(img->data + neighbor_idx);
                }

                int current_idx = (y * w + x) * channels + c;
                *(temp + current_idx) = (uint8_t) (sum / kernel_size);
            }
        }
    }

    for (int y = offset; y < h - offset; y++) {
        for (int x = offset; x < w - offset; x++) {
            for (int c = 0; c < channels; c++) {
                int sum = 0;

                for (int ky = -offset; ky <= offset; ky++) {
                    int neighbor_idx = ((y + ky) * w + x) * channels + c;
                    sum += *(temp + neighbor_idx);
                }

                int current_idx = (y * w + x) * channels + c;
                *(img->data + current_idx) = (uint8_t) (sum / kernel_size);
            }
        }
    }

    free(temp);
}

void filter_sobel(Image *img) {
    if (!img || !img->data) return;

    int w = img->width;
    int h = img->height;
    int channels = img->channels;
    int size = w * h * channels;
    
    uint8_t *temp = (uint8_t*) malloc(size * sizeof(uint8_t));
    if(!temp) return;

    uint8_t *src = img->data;
    uint8_t *dst = temp;
    for (int i = 0; i < size; i++) {
        *(dst + i) = *(src + i);
    }

    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int Gy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    for (int y = 1; y < h - 1; y++) {
        for (int x = 1; x < w - 1; x++) {
            int sum_x = 0;
            int sum_y = 0;

            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    int py = y + ky;
                    int px = x + kx;
                    int neighbor_idx = (py * w + px) * channels;
                    int pixel_val = *(temp + neighbor_idx);

                    sum_x += pixel_val * Gx[ky + 1][kx + 1];
                    sum_y += pixel_val * Gy[ky + 1][kx + 1];
                }
            }

            int magnitude = (int) sqrt((sum_x * sum_x) + (sum_y * sum_y));

            int current_idx = (y * w + x) * channels;
            *(img->data + current_idx) = clamp(magnitude);
        }
    }

    free(temp);
}

void filter_median(Image *img) {
    if (!img || !img->data) return;
    
    int w = img->width;
    int h = img->height;
    int channels = img->channels;
    int size = w * h * channels;

    uint8_t *temp = (uint8_t *) malloc(size * sizeof(uint8_t));
    if (!temp) return;

    uint8_t *src = img->data;
    uint8_t *dst = temp;
    for (int i = 0; i < size; i++) {
        *(dst + i) = *(src + i);
    }

    uint8_t window[9];

    for (int y = 1; y < h - 1; y++) {
        for (int x = 1; x < w - 1; x++) {
            int k = 0;
            for (int ky = -1; ky <= 1; ky++) {
                for(int kx = -1; kx <= 1; kx++) {
                    int neighbor_idx = ((y + ky) * w + (x + kx)) * channels;
                    window[k++] = *(temp + neighbor_idx);
                }
            }

            sort_window(window, 9);

            int current_idx = (y * w + x) * channels;
            *(img->data + current_idx) = window[4];
        }
    }

    free(temp);
}

void filter_threshold(Image *img, uint8_t threshold) {
    if (!img || !img->data) return;

    int w = img->width;
    int h = img->height;
    int channels = img->channels;
    int size = w * h * channels;
    
    uint8_t *p = img->data;

    for (int i = 0; i < size; i++) {
        *(p + i) = (*(p + i) >= threshold) ? 255 : 0;
    } 
}
