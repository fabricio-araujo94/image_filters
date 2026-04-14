#include "../include/image.h"
#include "../include/filters.h"
#include <stdio.h>

int main() {
    const char *input_path = "assets/input.jpg";
    const char *output_path = "assets/output_test.pgm";

    printf("Reading file: %s\n", input_path);
    Image *my_img = load_image_stb(input_path);

    if (my_img) {
        printf("Read. Dimensions: %dx%d\n", my_img->width, my_img->height);

        filter_negative(my_img);
        printf("Salving on: %s\n", output_path);

        if (save_pgm(output_path, my_img) == 0) {
            printf("Saved.");
        }
        free_image(my_img);
    } else {
        printf("Error opening the image. Please verify that the path exists.\n");
    }

    return 0;
}
