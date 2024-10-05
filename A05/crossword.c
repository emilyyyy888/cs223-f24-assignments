/*----------------------------------------------
 * Author: Emily Lu
 * Date: Oct 4 2024
 * Description: implement a program that generates a crossword using two words from a user. 
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <word1> <word2>\n", argv[0]);
        exit(0);
    }

    char* word1 = argv[1];
    char* word2 = argv[2];

    int rows = strlen(word1);
    int cols = strlen(word2);

    // Create the 2D matrix and fill it with dots
    char** matrix = (char**)malloc(rows * sizeof(char*));
    if (matrix == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    for (int i = 0; i < rows; i++) {
        matrix[i] = (char*)malloc(cols * sizeof(char));
        if (matrix[i] == NULL) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = '.';
        }
    }

    int rowPos = -1;
    int colPos = -1;
    int foundCommon = 0;

    // Find the position of the common letter
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (word1[i] == word2[j]) {
                rowPos = i;
                colPos = j;
                foundCommon = 1;
                break;
            }
        }
        if (foundCommon) break;  // Break outer loop if common letter found
    }

    if (!foundCommon) {
        printf("No common letter!\n");

        // Free the matrix before exiting
        for (int i = 0; i < rows; i++) {
            free(matrix[i]);
        }
        free(matrix);
        return 1;
    }

    // Fill in word1 vertically
    for (int i = 0; i < rows; i++) {
        matrix[i][colPos] = word1[i];
    }

    // Fill in word2 horizontally
    for (int i = 0; i < cols; i++) {
        matrix[rowPos][i] = word2[i];
    }

    // Print the matrix
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }

    // Free the matrix after use
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}
