#include <stdio.h>
#include <stdlib.h>

#define TOTAL_BLOCKS 20   // total disk blocks
#define SPARE_BLOCKS 5    // reserved spare blocks

// Structure for bad block table entry
struct BadBlockEntry {
    int bad_block;
    int spare_block;
};

int main() {
    int disk[TOTAL_BLOCKS]; // 0 = free, 1 = allocated, -1 = bad
    struct BadBlockEntry bad_table[SPARE_BLOCKS];
    int bad_count = 0;

    // Initialize disk: mark all as free
    for (int i = 0; i < TOTAL_BLOCKS; i++) {
        disk[i] = 0;
    }

    // Reserve last SPARE_BLOCKS as spare
    for (int i = TOTAL_BLOCKS - SPARE_BLOCKS; i < TOTAL_BLOCKS; i++) {
        disk[i] = 2; // mark spare
    }

    // Simulate some bad blocks
    disk[4] = -1;
    disk[10] = -1;

    printf("Disk initialized. Total blocks = %d, Spare blocks = %d\n", TOTAL_BLOCKS, SPARE_BLOCKS);

    // Build bad block table
    int spare_index = TOTAL_BLOCKS - SPARE_BLOCKS;
    for (int i = 0; i < TOTAL_BLOCKS - SPARE_BLOCKS; i++) {
        if (disk[i] == -1 && bad_count < SPARE_BLOCKS) {
            bad_table[bad_count].bad_block = i;
            bad_table[bad_count].spare_block = spare_index;
            spare_index++;
            bad_count++;
        }
    }

    // Print bad block table
    printf("\nBad Block Table:\n");
    printf("Bad Block -> Spare Block\n");
    for (int i = 0; i < bad_count; i++) {
        printf("%d -> %d\n", bad_table[i].bad_block, bad_table[i].spare_block);
    }

    // Simulate file allocation (contiguous)
    int start = 2, length = 6;
    printf("\nAllocating file of length %d starting at block %d\n", length, start);

    for (int i = start; i < start + length; i++) {
        int is_bad = 0;
        for (int j = 0; j < bad_count; j++) {
            if (bad_table[j].bad_block == i) {
                printf("Block %d is bad, using spare block %d instead.\n", i, bad_table[j].spare_block);
                disk[bad_table[j].spare_block] = 1;
                is_bad = 1;
                break;
            }
        }
        if (!is_bad) {
            disk[i] = 1;
        }
    }

    // Print disk status
    printf("\nDisk Status (index:status):\n");
    for (int i = 0; i < TOTAL_BLOCKS; i++) {
        printf("%2d:%2d  ", i, disk[i]);
        if ((i + 1) % 5 == 0) printf("\n");
    }

    return 0;
}

