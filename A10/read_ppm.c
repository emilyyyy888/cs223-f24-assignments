/*----------------------------------------------
 * Author: Emily Lu
 * Date: Oct 4 2024
 * Description: implement a function that can read PPM files stored in binary format
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// Choose *one* to implement (do not remove the other one!)

struct ppm_pixel* read_ppm(const char* filename, int* width, int* height) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("Error: Unable to open file %s\n", filename);
        return NULL;
    }

    char buffer[256];

    // Read magic number
    fgets(buffer, sizeof(buffer), fp); 

    // Skip comment line
    fgets(buffer, sizeof(buffer), fp); 

    // Read ASCII data
    fscanf(fp, "%d %d", width, height);

    // Read max color value
    int max_val;
    fscanf(fp, "%d", &max_val);

    // Read the single whitespace character
    fgetc(fp);

    // Allocate memory for pixel data and read pixel data
    struct ppm_pixel* pixels = (struct ppm_pixel*)malloc((*width) * (*height) * sizeof(struct ppm_pixel));
    if (!pixels) {
        printf("Error: Unable to allocate memory for pixel data\n");
        fclose(fp);
        return NULL;
    }
    
    size_t num_pixels = (*width) * (*height);
    fread(pixels, sizeof(struct ppm_pixel), num_pixels, fp); // Read from file fp pixels of num_pixels,
                                                             // where each pixel is struct ppm_pixel, 
                                                             // store the value into pixels array

    fclose(fp);
    return pixels;
}

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
    return NULL;
}