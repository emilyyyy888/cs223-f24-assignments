/*----------------------------------------------
 * Author: Emily Lu
 * Date: Oct 4 2024
 * Description: Implements a function to write PPM files in binary format.
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>
#include "read_ppm.h" 

// implement *one* (but do not remove the other one)!

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
    FILE *fp = fopen(filename, "wb");  
    if (!fp) {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }

    //write ascii data
    fprintf(fp, "P6\n");                   // Magic number for binary PPM
    fprintf(fp, "#Created by write_ppm function\n");
    fprintf(fp, "%d %d\n", w, h); // Image dimensions
    fprintf(fp, "255\n");                   // Max color value

    //write binary data
    size_t num_pixels = w * h;
    fwrite(pixels, sizeof(struct ppm_pixel), num_pixels, fp);

    fclose(fp);
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

}
