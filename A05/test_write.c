/*----------------------------------------------
 * Author: Emily Lu
 * Date: Oct 4 2024
 * Description: Tests the write_ppm function.
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
    int w, h;
    struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);
    
    if (!pixels) {
        return 1;
    }

    printf("Testing file feep_raw.ppm: %d %d\n", w, h);

    // Write the pixels to a new file
    write_ppm("test.ppm", pixels, w, h);

    // Reload the test.ppm file
    struct ppm_pixel* test_pixels = read_ppm("test.ppm", &w, &h);
    if (!test_pixels) {
        free(pixels); 
        return 1; 
    }

    // Print the contents of the test.ppm
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            struct ppm_pixel pixel = test_pixels[i * w + j];
            printf("(%d,%d,%d) ", pixel.red, pixel.green, pixel.blue);
        }
        printf("\n");
    }

    free(test_pixels);
    free(pixels);
    return 0;
}
