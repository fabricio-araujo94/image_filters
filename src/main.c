#include "../include/image.h"
#include "../include/filters.h"
#include <stdio.h>


int main() {
    const char *path = "assets/input.jpg";

    Image *img = load_image_stb(path);

    if (img) {
        printf("Removendo ruidos...");
        filter_separable_blur(img, 5);

        printf("Encontrando os contornos...");
        filter_sobel(img);

        uint8_t limiar = 50;
        printf("Binarizando a imagem...");
        filter_threshold(img, limiar);

        save_pgm("assets/segmented_result.pgm", img);
        free_image(img);

        puts("Feito!");
    } else {
        printf("Erro ao carregar a imagem.\n");
    }
    
    return 0;
}
