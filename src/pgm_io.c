#include "../include/image.h"
#include <stdio.h>
#include <stdlib.h>

Image* create_image(int w, int h, int c) {
	Image *img = (Image*) malloc(sizeof(Image));
    img->width = w;
    img->height = h;
    img->channels = c;
    img->data = (uint8_t*) malloc(w * h * c * sizeof(uint8_t));
    return img;
}

void free_image(Image *img) {
  if (img) {
    free(img->data);
		free(img);
	}
}

Image* read_pgm(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return NULL;

    char magic[3];
    int w, h, max_val;

    fscanf(fp, "%s\n%d %d\n%d\n", magic, &w, &h, &max_val);

    if (magic[0] != 'P' || magic[1] != '5') {
        printf("Error: only binary PGM (P5) is supported.\n");
        fclose(fp);
    	return NULL;
    }

    Image *img = create_image(w, h, 1);

    fread(img->data, sizeof(uint8_t), w * h, fp);

    fclose(fp);
    return img;
}

int save_pgm(const char *filename, const Image *img) {
    FILE *fp = fopen(filename, "wb");
	if (!fp) return -1;

	fprintf(fp, "P5\n%d %d\n255\n", img->width, img->height);

	fwrite(img->data, sizeof(uint8_t), img->width * img->height, fp);

    fclose(fp);
	return 0;
}
