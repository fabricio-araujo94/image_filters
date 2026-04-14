#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

typedef struct {
	int width;
	int height;
	int channels;
	uint8_t *data;
} Image;

Image* read_pgm(const char *filename);
int save_pgm(const char *filename, const Image *img);

Image* load_image_stb(const char *filename);

Image* create_image(int w, int h, int c);
void free_image(Image *img);

#endif
