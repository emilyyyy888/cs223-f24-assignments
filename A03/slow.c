/*----------------------------------------------
 * Author: Emily Lu
 * Date: September 20, 2024
 * Description: This program generates an "Ent-speech" version of the input.
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    char buff[32];         
    int pauseLen;          
    char *entPhrase;       

    printf("Pause length: ");
    scanf("%d", &pauseLen);

    printf("Text: ");
    scanf("%s", buff);

    int inputLength = strlen(buff);
    int outputLength = inputLength + inputLength * pauseLen;

    // Allocate memory for the Ent-speech phrase
    entPhrase = malloc(outputLength * sizeof(char));
    if (entPhrase == NULL) {
        printf("Memory allocation failed.\n");
        return 1; 
    }

    int entIndex = 0;
    for (int i = 0; i < inputLength; i++) {
        entPhrase[entIndex++] = buff[i]; 

        // Add dots after each character
        for (int j = 0; j < pauseLen; j++) {
            entPhrase[entIndex++] = '.';
        }
    }

    entPhrase[entIndex] = '\0';

    printf("%s\n", entPhrase);

    free(entPhrase);

    return 0; 
}
