#define STB_IMAGE_IMPLEMENTATION
#include "../include/image.h"
#include "../include/stb_image.h"

#include <stdio.h>

Image* load_image_stb(const char *filename) {
    int w, h, c;
    unsigned char *data = stbi_load(filename, &w, &h, &c, 1);

    if (!data) {
        printf("Erro ao carregar imagem com STB: %s\n", filename);
        return NULL;
    }

    Image *img = create_image(w, h, 1);

    uint8_t *src = data;
    uint8_t *dest = img->data;
    for(int i = 0; i < w * h; i++) {
        *(dest + i) = *(src + i);
    }

    stbi_image_free(data);
    return img;
}
