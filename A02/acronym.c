/**
 * acronym.c
 * This program reads a phrase from the user and generates an acronym.
 * 
 * @author: Emily Lu
 * @version: September 13, 2024
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_uppercase(char c) {
    return (c >= 'A' && c <= 'Z');
}

int main() {
    char phrase[1024];
    char acronym[1024];
    int acronym_index = 0;

    printf("Enter a phrase: ");
    // Read the input phrase
    scanf("%[^\n]%*c", phrase);

    // Process each character
    for (int i = 0; i < strlen(phrase); i++) {
        if (is_uppercase(phrase[i])) {
            // Add uppercase letters to the acronym array
            acronym[acronym_index++] = phrase[i];
        }
    }

    acronym[acronym_index] = '\0';

    printf("Your acronym is %s\n", acronym);

    return 0;
}
