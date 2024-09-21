/*----------------------------------------------
 * Author: Emily Lu
 * Date: September 20, 2024
 * Description: A program that reads songs from a text file into an array.
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char title[128];
    char artist[128];
    int duration;          
    float danceability;  
    float energy;        
    float tempo;          
    float valence;        
} Song;

/**
 * Parses a line from the CSV file and converts it into a Song structure.
 * @param line A string representing one line from the CSV file.
 * @return A Song struct with parsed information from the line.
 */
Song parseSong(char *line) {
    Song song;

    char *token = strtok(line, ",");
    strcpy(song.title, token);

    token = strtok(NULL, ",");
    strcpy(song.artist, token);

    token = strtok(NULL, ",");
    song.duration = atoi(token); // Convert duration to integer

    token = strtok(NULL, ",");
    song.danceability = atof(token); // Convert danceability to float

    token = strtok(NULL, ",");
    song.energy = atof(token); 

    token = strtok(NULL, ",");
    song.tempo = atof(token); 

    token = strtok(NULL, ",");
    song.valence = atof(token); 

    return song;
}

/**
 * Prints the list of songs in a formatted manner.
 * @param songs An array of Song structs to be printed.
 * @param numSongs The total number of songs to be printed.
 */
void printSongs(Song *songs, int numSongs) {
    printf("Welcome to Dynamic Donna's Danceability Directory.\n\n");
    for (int i = 0; i < numSongs; i++) {
        int seconds = songs[i].duration / 1000;
        int minutes = seconds / 60;
        seconds = seconds % 60;

        printf("%d) %-20s artist: %-20s duration: %d:%02d D: %.3f E: %.3f T: %.3f V: %.3f\n",
            i,
            songs[i].title,
            songs[i].artist,
            minutes,
            seconds,
            songs[i].danceability,
            songs[i].energy,
            songs[i].tempo,
            songs[i].valence);
    }
}

int main() {
    FILE *file = fopen("songlist.csv", "r");
    if (file == NULL) {
        printf("Error: unable to open file %s\n", "songlist.csv");
        exit(1);
    }

    char buffer[1024];

    // Read the first line to get the number of songs
    fgets(buffer, sizeof(buffer), file);
    int numSongs = atoi(strtok(buffer, ","));

    // Skip the second line (header)
    fgets(buffer, sizeof(buffer), file);

    // Allocate memory for the songs
    Song *songs = malloc(numSongs * sizeof(Song));
    if (songs == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
        exit(1);
    }

    // Read each song line and parse it into the songs array
    for (int i = 0; i < numSongs; i++) {
        fgets(buffer, sizeof(buffer), file);
        songs[i] = parseSong(buffer);
    }

    printSongs(songs, numSongs);

    free(songs);
    fclose(file);

    return 0; 
}
