/*----------------------------------------------
 * Author: Emily Lu
 * Date: Oct 12
 * Description:  Implement a program that reads in a single 64-bit unsigned integer 
 * (e.g. unsigned long) and outputs it as an 8x8 1bpp sprite
 ---------------------------------------------*/
#include <stdio.h>

int main() {
  unsigned long img;
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n", img);

  for(int row = 0; row < 8; row++) {
    for(int col = 0; col < 8; col++){
      int bitPosition = 63 - (row * 8 + col);

      unsigned long mask = 0x1ul << bitPosition;

      if(img & mask) {
        printf("@ "); //the bit is 1
      } else {
        printf("  ");//the bit is 0
      }
    }
    printf("\n");
  }
  return 0;
}
