/*----------------------------------------------
* Author: Emily Lu
* Date: 11/8/2024
* Description: compute a program that outputs a PPM image of the mandelbrot set
---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your work here
  srand(time(0));

  struct ppm_pixel* pixels = malloc(size * size * sizeof(struct ppm_pixel));
  if (!pixels) {
    printf("Error: Cannot allocate memory.\n");
    return 1;
  }
  
  struct ppm_pixel* palette = malloc(sizeof(struct ppm_pixel) * maxIterations);
  if (!palette) {
    printf("Error: Cannot allocate memory.\n");
    return 1;
  }

  //random colors
  for (int i = 0; i < maxIterations; i++) {
    palette[i].red = rand() % 255;
    palette[i].green = rand() % 255;
    palette[i].blue = rand() % 255;
  }

  struct timeval tstart, tend; 
  gettimeofday(&tstart, NULL);

  for (int row = 0; row < size;row++) {
    for (int col = 0; col < size; col++) {
        // Map pixel position to complex plane
        float xfrac = (float)row / size;
        float yfrac = (float)col /size;
        float x0 = xmin + xfrac * (xmax - xmin);
        float y0 = ymin + yfrac * (ymax - ymin);
        float x = 0.0;
        float y = 0.0;
        int iter = 0;

        // Mandelbrot iteration
        while (x * x + y * y < 4.0 && iter < maxIterations) {
            float temp = x * x - y * y + x0;
            y = 2 * x * y + y0;
            x = temp;
            iter++;
        }

        if (iter < maxIterations) { 
          //not in the set, assign a color based on iter
          pixels[row * size + col] = palette[iter];
        } else {
          //the point is inside the set, assign black color to it
          pixels[row * size + col].red = 0;
          pixels[row * size + col].blue = 0;
          pixels[row * size + col].green = 0;

        }
    }
  }

  // compute the time 
  gettimeofday(&tend, NULL);
  double timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, timer);

  // writing output file 
  char filename[256];
  snprintf(filename, sizeof(filename), "mandelbrot-%d-%ld.ppm", size, time(0));
  printf("Writing file: %s\n", filename);
  write_ppm(filename, pixels, size, size);

  free(pixels);
  free(palette);
  return 0;
}