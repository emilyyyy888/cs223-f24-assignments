/*----------------------------------------------
 * Author: Emily Lu
 * Date: Oct 13 2024
 * Description: This program reads in a PPM file (raw, or binary, format) and
 *  then outputs any message that might be stored in the least significant bits of each color.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: decode <file.ppm>\n");
    return 0;
  }

  char* filename = argv[1];

  FILE *file = fopen(filename, "rb");
  if (!file) {
    printf("Error: Unable to open file");
    return 1;
  }

  int width, height;
  struct ppm_pixel *pixels = read_ppm(filename, &width, &height);;
  fclose(file);
  printf("Reading %s with width %d and height %d\n", filename, width, height);

  int maxChar = (width * height * 3) / 8; //each pixel is 3 bits, and each character takes 8 bits
  printf("Max number of characters in the image: %d\n", maxChar);

  char* message = (char*)malloc((maxChar + 1) * sizeof(char));
  if(!message){
    printf("Memory allocation failed!\n");
    free(pixels);
    return 1;
  }


  int bitIndex = 0;
  int charIndex = 0;
  unsigned char curr = 0;

  for (int i = 0; i < width * height * 3; i++) {
    unsigned char* pixelBytes = (unsigned char*)pixels; //cast into array of bytes
    unsigned char lsb = pixelBytes[i] & 0x1; //get the lsb

    curr |= (lsb << (7 - bitIndex));//place the lsb into the right position
    bitIndex++;

    if (bitIndex == 8) {
      if (curr == '\0') {
        break;
      }
      message[charIndex] = curr;
      charIndex++;
      bitIndex = 0;
      curr = 0;
    }
  }
  
  message[charIndex] = '\0'; //null-terminate
  printf("%s", message);

  free(pixels);
  free(message);
  return 0;
}

