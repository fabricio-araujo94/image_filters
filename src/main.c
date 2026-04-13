#include "../include/image.h"
#include <stdio.h>

int main() {
    const char *input_path = "assets/input.pgm";
    const char *output_path = "assets/output_test.pgm";

    print("Reading file: %s\n", input_path);
    Image *my_img = read_pgm(input_path);

    if (my_img) {
        printf("Read. Dimensions: %dx%d\n", my_img->width, my_img->height);
        printf("Salving a copy on: %s\n", output_path);

        if (save_pgm(output_path, my_img) == 0) {
            printf("Saved.");
        }
        free_image(my_img);
    } else {
        printf("Error opening the image. Please verify that the path exists.\n");
    }

    return 0;
}
