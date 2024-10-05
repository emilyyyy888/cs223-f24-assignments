/*----------------------------------------------
 * Author: Emily Lu
 * Date: Oct 4 2024 
 * Description: This program calls the function in read_ppm.c and prints the contents of feep-raw.ppm.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main() {
    int w, h;
    const char* filename = "feep-raw.ppm";
    struct ppm_pixel* pixels = read_ppm(filename, &w, &h);
    
    if (!pixels) {
        return 1;  // Error occurred
    }
    
    printf("Testing file %s: %d %d\n", filename, w, h);
    // TODO: print out the grid of pixels

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            struct ppm_pixel pixel = pixels[i * w + j];
            printf("(%d,%d,%d) ", pixel.red, pixel.green, pixel.blue);
        }
        printf("\n");
    }

    free(pixels);
    return 0;
}
