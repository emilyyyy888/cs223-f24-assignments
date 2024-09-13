/**
 * exclaim.c
 * This program takes an input string from the user and replaces
 * all lowercase letters in the string with random special characters
 * from the set {'@', '!', '#', '*'}.
 * 
 * @author: Emily Lu
 * @version: September 13, 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Function to get a random character from the set {'@', '!', '#', '*'}
char random_char() {
    char chars[] = {'@', '!', '#', '*'};
    return chars[rand() % 4];
}

// Function to check if a character is a lowercase
int isLower(char c) {
  return (c >= 'a' && c <= 'z');
}

int main() {
    char buff[32];
    printf("Enter a word: ");
    scanf("%s", buff);  
    
    int length = strlen(buff);
    // Process each character in the input buffer
    for (int i = 0; i < length; i++) {
        if (isLower(buff[i])) {
            // Replace lowercase letters with random characters
            buff[i] = random_char();
        }
    }
    
    // Print the modified string
    printf("OMG! %s\n", buff);
    
    return 0;
}
