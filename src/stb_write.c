#include "../include/image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

#include <stdio.h>

int save_image_jpg(const char *filename, const Image *img, int quality) {
    if (!img || !img->data) {
        printf("Error: Empty image or no data to save.\n");
        return 0;
    }

    int success = stbi_write_jpg(filename, img->width, img->height, img->channels, img->data, quality);
    return success;
}

int save_image_png(const char *filename, const Image *img) {
    if (!img || !img->data) {
        printf("Erro: Imagem vazia ou sem dados para salvar.\n");
        return 0;
    }

    int stride = img->width * img->channels;
    int success = stbi_write_png(filename, img->width, img->height, img->channels, img->data, stride);
    return success;
}
