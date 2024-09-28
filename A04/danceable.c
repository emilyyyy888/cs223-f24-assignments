/*----------------------------------------------
 * Author: Emily Lu
 * Date: Sep 27 2024
 * Description: A program to manage a linked list of songs, allowing insertion, 
 *              display, and removal of the most danceable song from a CSV file.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/** 
 * Structure representing a song.
 */
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
 * Structure representing a node in the linked list.
 */
typedef struct Node {
    Song song;
    struct Node* next;
} Node;

/**
 * Parses a CSV line to create a Song object.
 * @param line A pointer to a string containing the CSV line.
 * @return A Song object populated with the parsed data.
 */
Song parseSong(char *line) {
    Song song;
    char *token = strtok(line, ",");
    strcpy(song.title, token);

    token = strtok(NULL, ",");
    strcpy(song.artist, token);

    token = strtok(NULL, ",");
    song.duration = atoi(token);

    token = strtok(NULL, ",");
    song.danceability = atof(token);

    token = strtok(NULL, ",");
    song.energy = atof(token);

    token = strtok(NULL, ",");
    song.tempo = atof(token);

    token = strtok(NULL, ",");
    song.valence = atof(token);

    return song;
}

/**
 * Inserts a new song into the linked list.
 * @param head A pointer to the head of the linked list.
 * @param song The song to insert.
 * @return The new head of the linked list.
 */
Node* insertSong(Node* head, Song song) {
    Node* newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("ERROR:Out of Space\n");
        exit(1);
    }
    newNode->song = song;
    newNode->next = head;
    return newNode;
}

/**
 * Prints all songs in the linked list.
 * @param head A pointer to the head of the linked list.
 */
void printSongs(Node* head) {
    int index = 0;
    while (head) {
        int seconds = head->song.duration / 1000;
        int minutes = seconds / 60;
        seconds = seconds % 60;

        printf("%d) %-30s %-20s (%d:%02ds) D: %.3f E: %.3f T: %.3f V: %.3f\n",
            index++,
            head->song.title,
            head->song.artist,
            minutes,
            seconds,
            head->song.danceability,
            head->song.energy,
            head->song.tempo,
            head->song.valence);
        
        head = head->next;
    }
    printf("Dataset contains %d songs\n", index);
}

/**
 * Finds and removes the most danceable song from the linked list.
 * @param head A pointer to the pointer to the head of the linked list.
 * @return The node of the most danceable song that was removed, or NULL if the list is empty.
 */
Node* findAndRemoveMostDanceable(Node **head) {
    if (*head == NULL) return NULL;

    Node *current = *head;
    Node *prev = NULL;
    Node *mostDanceableNode = *head;
    Node *mostDanceablePrev = NULL;

    while (current) {
        if (current->song.danceability > mostDanceableNode->song.danceability) {
            mostDanceableNode = current;
            mostDanceablePrev = prev;
        }
        prev = current;
        current = current->next;
    }

    // Remove the most danceable node
    if (mostDanceablePrev) {
        mostDanceablePrev->next = mostDanceableNode->next;
    } else {
        *head = mostDanceableNode->next; // Update head if it's the first node
    }

    return mostDanceableNode;
}

/**
 * Clears the entire linked list and frees the allocated memory.
 * @param head A pointer to the pointer to the head of the linked list.
 */
void clearList(Node **head) {
    Node *current = *head; // Dereference to get the actual head
    Node *nextNode;

    while (current != NULL) {
        nextNode = current->next; // Save the next node
        free(current);            // Free the current node
        current = nextNode;       // Move to the next node
    }
    *head = NULL; // Set head to NULL after clearing the list
}


int main() {
    FILE *file = fopen("songlist.csv", "r");
    if (file == NULL) {
        printf("Error: unable to open file %s\n", "songlist.csv");
        exit(1);
    }

    char buffer[1024];
    Node *head = NULL;

    // Skip the header line
    fgets(buffer, sizeof(buffer), file);

    // Read each song line and parse it into the linked list
    while (fgets(buffer, sizeof(buffer), file)) {
        Song song = parseSong(buffer);
        head = insertSong(head, song);
    }

    fclose(file);

    char input;
    while (1) {
        printSongs(head);
        printf("\n=======================\n");
        printf("Press 'd' to show the most danceable song (any other key to quit): ");
        scanf(" %c", &input);

        if (input == 'd') {
            Node *mostDanceable = findAndRemoveMostDanceable(&head);
            if (mostDanceable) {
                printf("---------------------------------------- Most danceable ------------------------------------\n");
                printf("%-30s %-20s (%d:%02ds) D: %.3f E: %.3f T: %.3f V: %.3f\n",
                    mostDanceable->song.title,
                    mostDanceable->song.artist,
                    mostDanceable->song.duration / 1000 / 60,
                    (mostDanceable->song.duration / 1000) % 60,
                    mostDanceable->song.danceability,
                    mostDanceable->song.energy,
                    mostDanceable->song.tempo,
                    mostDanceable->song.valence);
                printf("--------------------------------------------------------------------------------------------\n");
                free(mostDanceable);
            }
        } else {
            break; // Exit the loop on any key other than 'd'
        }
    }

    clearList(&head);
    return 0;
}