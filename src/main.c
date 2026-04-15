#include "../include/image.h"
#include "../include/filters.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clear_input_buffer() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

int main() {
    char path[256];

    printf("=======================================\n");
    printf("      IMAGE PROCESSOR IN C            \n");
    printf("=======================================\n\n");

    printf("Enter the image path: ");
    if (fgets(path, sizeof(path), stdin) != NULL) {
        path[strcspn(path, "\n")] = '\0';
    }

    Image *img = load_image_stb(path);
    if (!img) {
        printf("\n[ERROR] Failed to load image. Check if the file exists and try again.\n");
        return 1;
    }

    printf("\n[INFO] Image loaded: %dx%d (%d channels)\n", img->width, img->height, img->channels);

    int option = -1;
    while (option != 0) {
        printf("\n--- FILTER MENU ---\n");
        printf("1. Negative\n");
        printf("2. Brightness\n");
        printf("3. Contrast\n");
        printf("4. Mean Blur (3x3)\n");
        printf("5. Separable Blur (Choose Kernel)\n");
        printf("6. Edge Detection (Sobel)\n");
        printf("7. Median Filter (Remove Salt/Pepper Noise)\n");
        printf("8. Binarization (Threshold)\n");
        printf("-----------------------\n");
        printf("0. Save and Exit\n");
        printf("9. Exit without saving\n");

        printf("\nEnter a number: ");
        if (scanf("%d", &option) != 1) {
            clear_input_buffer();
            printf("\n[ERROR] Invalid input. Enter a number.\n");
            continue;
        }

        switch (option) {
            case 1:
                filter_negative(img);
                printf("\n[SUCCESS] Negative filter applied.\n");
                break;

            case 2: {
                int amount;
                printf("Enter the brightness value (+ to lighten, - to darken): ");
                scanf("%d", &amount);
                filter_brightness(img, amount);
                printf("\n[SUCCESS] Brightness (%d) applied.\n", amount);
                break;
            }

            case 3: {
                float factor;
                printf("Enter the contrast factor (e.g., 1.5 increases 50%%, 0.8 decreases 20%%): ");
                scanf("%f", &factor);
                filter_contrast(img, factor);
                printf("\n[SUCCESS] Contrast (%.2fx) applied.\n", factor);
                break;
            }

            case 4:
                filter_mean_blur(img, 3);
                printf("\n[SUCCESS] Mean Blur 3x3 applied.\n");
                break;

            case 5: {
                int k;
                printf("Enter the odd kernel size (e.g., 5, 7, 15): ");
                scanf("%d", &k);
                filter_separable_blur(img, k);
                printf("\n[SUCCESS] Separable Blur %dx%d applied.\n", k, k);
                break;
            }

            case 6:
                filter_sobel(img);
                printf("\n[SUCCESS] Sobel filter (Edges) applied.\n");
                break;

            case 7:
                filter_median(img);
                printf("\n[SUCCESS] Median filter applied.\n");
                break;

            case 8: {
                int thresh;
                printf("Enter the threshold value (0 to 255): ");
                scanf("%d", &thresh);
                filter_threshold(img, (uint8_t) thresh);
                printf("\n[SUCCESS] Binarization (Threshold: %d) applied.\n", thresh);
                break;
            }
            case 0:
                break;

            case 9:
                printf("\nExiting without saving changes...\n");
                free_image(img);
                return 0;
            
            default:
                printf("\n[ERROR] Invalid option.\n");
        }
    }

    char out_path[512];
    char *dot = strrchr(path, '.');

    if (dot) {
        int len = dot - path;
        strncpy(out_path, path, len);
        out_path[len] = '\0';
        strcat(out_path, "_edited.jpg");
    } else {
        strcpy(out_path, path);
        strcat(out_path, "_edited.jpg");
    }

    printf("\nSaving image to: %s\n", out_path);

    if (save_image_jpg(out_path, img, 95)) {
        printf("[SUCCESS] Image saved with 95%% quality.\n");
    } else {
        printf("[ERROR] Failed to save the file.\n");
    }

    free_image(img);

    return 0;
}
