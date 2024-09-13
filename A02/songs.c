/**
 * songs.c
 * Implements a simple song management system where users can
 * view and edit details of a list of songs.
 * 
 * @author: Emily Lu
 * @version: September 13, 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Define a struct to hold song information
typedef struct {
    char title[32];
    char artist[32];
    int duration_minutes;
    int duration_seconds;
    float danceability;
} Song;

// Function to print all songs
void print_songs(Song songs[], int count) {
    printf("\n");
    for (int i = 0; i < count; i++) {
        printf("%d) %-20s artist: %-20s duration: %d:%02d  danceability: %.2f\n",
            i,
            songs[i].title,
            songs[i].artist,
            songs[i].duration_minutes,
            songs[i].duration_seconds,
            songs[i].danceability);
    }
    printf("\n=======================\n");
}

// Function to edit the song
void edit_song(Song songs[], int index) {
    char attribute[32];
    printf("Which attribute do you wish to edit? [artist, title, duration, danceability]: ");
    scanf(" %[^\n]%*c", attribute);

    if (strcmp(attribute, "artist") == 0) {
        printf("Enter a new artist: ");
        scanf(" %[^\n]%*c", songs[index].artist);
    } else if (strcmp(attribute, "title") == 0) {
        printf("Enter a new title: ");
        scanf(" %[^\n]%*c", songs[index].title);
    } else if (strcmp(attribute, "duration") == 0) {
        printf("Enter a duration (minutes): ");
        scanf("%d%*c", &songs[index].duration_minutes);
        printf("Enter a duration (seconds): ");
        scanf("%d%*c", &songs[index].duration_seconds);
    } else if (strcmp(attribute, "danceability") == 0) {
        printf("Enter danceability: ");
        scanf("%f%*c", &songs[index].danceability);
    } else {
        printf("Invalid attribute!\n");
    }
}

int main() {
    Song songs[3] = {
        {"Shout", "Tears for Fears", 4, 11, 0.50},
        {"As it was", "Harry Styles", 2, 47, 0.70},
        {"Wish you were here", "Pink Floyd", 5, 34, 0.30}
    };

    int choice;
    printf("Welcome to Steven Struct's Song List.\n");
    print_songs(songs, 3);

    printf("Enter a song id to edit [%d,%d,%d]: ", 0, 1, 2);
    scanf("%d%*c", &choice);

    if (choice >= 0 && choice < 3) {
        edit_song(songs, choice);
        print_songs(songs, 3);
    } else {
        printf("Invalid choice!\n");
    }

    return 0;
}
