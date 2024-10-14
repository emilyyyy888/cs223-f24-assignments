/*----------------------------------------------
 * Author: Emily Lu
 * Date: Oct 13 2024
 * Description:This program reads in a PPM file and encodes a message 
 * into the least significant bits of each color channel.
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: encode <file.ppm>\n");
    return 0;
  }

  char* filename = argv[1];
  int width, height;
  struct ppm_pixel* pixels = read_ppm(filename, &width, &height);

  if(!pixels){
    printf("Error: Cannot read file %s\n", filename);
    return 1;
  }

  printf("Reading %s with width %d and height %d\n", filename, width, height);

  int max_chars = (width * height * 3) / 8 - 1;
  printf("Max number of characters in the image: %d\n", max_chars);

  char* message = (char*)malloc((max_chars + 1) * sizeof(char));
  if (message == NULL) {
    printf("Error: Unable to allocate memory for message.\n");
    free(pixels);
    return 1;
  }

  printf("Enter a phrase: ");
  scanf("%[^\n]", message);

  int message_len = strlen(message);

  int bit_index = 0;
  for(int i = 0; i < message_len; i++) {
    for (int bit = 0; bit < 8; bit++) {
      unsigned char* pixel_bytes = (unsigned char*)pixels;
      unsigned char current_bit = (message[i] >> bit) & 1;

      if(bit_index >= width * height * 3){
        printf("Error: Message is too long to fit in the image.\n");
        return 1;
      }

      //sets the LSB to 0 and leave other bits unchanged and then sets the LSB to value of current_bit
      pixel_bytes[bit_index] = (pixel_bytes[bit_index] & 0xFE) | current_bit;
      bit_index++;
    }
  }

  char output[256];
  strncpy(output, filename,sizeof(output) - 1);
  output[sizeof(output) - 1] = '\0';

  char *dot = strrchr(output, '.');

  if(dot) {
    snprintf(dot, sizeof(output) - (dot - output),"-encoded.ppm"); //concatenate the filename
  }
  write_ppm(output, pixels, width, height);
  printf("Writing file %s\n", output);

  free(message);
  free(pixels);
  return 0;
}

