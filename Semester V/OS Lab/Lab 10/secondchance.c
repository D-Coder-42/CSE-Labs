#include <stdio.h>
#include <stdlib.h>

#define MAX_FRAMES 4
#define MAX_REFERENCES 100

typedef struct {
    int page;
    int ref_bit;
} Frame;

// Function declarations
void initializeFrames(Frame frames[], int frame_count);
void inputPageReferences(int page_refs[], int *num_refs);
int isPageInFrames(Frame frames[], int page, int frame_count);
void handlePageFault(Frame frames[], int page, int *pointer, int frame_count);
void printFrames(Frame frames[], int frame_count);
void simulateSecondChance(Frame frames[], int page_refs[], int num_refs, int frame_count, int *page_faults, int *hits);

// Main function
int main() {
    int page_refs[MAX_REFERENCES];
    int num_refs;
    int frame_count = MAX_FRAMES;
    Frame frames[MAX_FRAMES];
    int page_faults = 0, hits = 0;

    initializeFrames(frames, frame_count);
    inputPageReferences(page_refs, &num_refs);
    simulateSecondChance(frames, page_refs, num_refs, frame_count, &page_faults, &hits);

    // Results
    printf("\nTotal Page Faults: %d\n", page_faults);
    printf("Total Hits: %d\n", hits);
    printf("Hit Ratio: %.2f\n", (float)hits / num_refs);

    return 0;
}

// Initialize frame array
void initializeFrames(Frame frames[], int frame_count) {
    for (int i = 0; i < frame_count; i++) {
        frames[i].page = -1;
        frames[i].ref_bit = 0;
    }
}

// Input page reference string
void inputPageReferences(int page_refs[], int *num_refs) {
    printf("Enter number of page references: ");
    scanf("%d", num_refs);
    printf("Enter page reference string:\n");
    for (int i = 0; i < *num_refs; i++) {
        scanf("%d", &page_refs[i]);
    }
}

// Check if page is in frames
int isPageInFrames(Frame frames[], int page, int frame_count) {
    for (int i = 0; i < frame_count; i++) {
        if (frames[i].page == page) {
            frames[i].ref_bit = 1;  // Set reference bit on hit
            return 1;
        }
    }
    return 0;
}

// Handle page fault using second chance
void handlePageFault(Frame frames[], int page, int *pointer, int frame_count) {
    while (1) {
        if (frames[*pointer].ref_bit == 0) {
            printf("Replacing page %d with %d\n", frames[*pointer].page, page);
            frames[*pointer].page = page;
            frames[*pointer].ref_bit = 1;
            *pointer = (*pointer + 1) % frame_count;
            break;
        } else {
            printf("Giving second chance to page %d\n", frames[*pointer].page);
            frames[*pointer].ref_bit = 0;
            *pointer = (*pointer + 1) % frame_count;
        }
    }
}

// Print current frame state
void printFrames(Frame frames[], int frame_count) {
    printf("Frames: ");
    for (int i = 0; i < frame_count; i++) {
        if (frames[i].page != -1)
            printf("[%d|%d] ", frames[i].page, frames[i].ref_bit);
        else
            printf("[--] ");
    }
    printf("\n");
}

// Simulate second chance algorithm
void simulateSecondChance(Frame frames[], int page_refs[], int num_refs, int frame_count, int *page_faults, int *hits) {
    int pointer = 0;

    for (int i = 0; i < num_refs; i++) {
        int page = page_refs[i];
        printf("\nReference to page %d:\n", page);

        if (isPageInFrames(frames, page, frame_count)) {
            (*hits)++;
            printf("→ HIT\n");
        } else {
            (*page_faults)++;
            printf("→ PAGE FAULT\n");
            handlePageFault(frames, page, &pointer, frame_count);
        }

        printFrames(frames, frame_count);
    }
}
