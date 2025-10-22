#include <stdio.h>
#include <stdlib.h>

#define NUM_SEGMENTS 5

// Segment Table Structure
struct SegmentTable {
    int segment_number;
    int base_address;   // Starting physical address
    int limit;          // Maximum offset allowed
};

typedef struct SegmentTable SegmentTable;

// Global segment table
SegmentTable seg_table[NUM_SEGMENTS] = {
    {0, 1400, 1000},  // Segment 0: base=1200, limit=1000
    {1, 6300, 400},   // Segment 1: base=6300, limit=400
    {2, 4300, 400},   // Segment 2: base=4300, limit=400
    {3, 3200, 1100},  // Segment 3: base=3200, limit=1100
    {4, 4700, 1000}   // Segment 4: base=4700, limit=1000
};

// Function to display segment table
void printSegmentTable() {
    printf("=== SEGMENT TABLE ===\n");
    printf("Segment#\tBase\tLimit\n");
    printf("-------\t----\t-----\n");
    for (int i = 0; i < NUM_SEGMENTS; i++) {
        printf("  %d    \t%d\t%d\n", 
               seg_table[i].segment_number, 
               seg_table[i].base_address, 
               seg_table[i].limit);
    }
    printf("\n");
}

// Function to convert logical address to physical address
int logicalToPhysical(int segment_num, int offset) {
    // Validate segment number
    if (segment_num < 0 || segment_num >= NUM_SEGMENTS) {
        printf("ERROR: Invalid segment number %d\n", segment_num);
        return -1;
    }
    
    // Check offset against limit
    if (offset < 0 || offset >= seg_table[segment_num].limit) {
        printf("ERROR: Offset %d out of bounds for segment %d (limit=%d)\n", 
               offset, segment_num, seg_table[segment_num].limit);
        return -1;
    }
    
    // Calculate physical address: base + offset
    int physical_addr = seg_table[segment_num].base_address + offset;
    return physical_addr;
}

// Function to test specific addresses
void testAddresses() {
    printf("=== ADDRESS TRANSLATION RESULTS ===\n");
    
    // (i) 53 byte of segment 2
    printf("Segment 2, Offset 53: ");
    int addr1 = logicalToPhysical(2, 53);
    if (addr1 != -1) {
        printf("Physical Address = %d\n", addr1);
    }
    
    // (ii) 852 byte of segment 3
    printf("Segment 3, Offset 852: ");
    int addr2 = logicalToPhysical(3, 852);
    if (addr2 != -1) {
        printf("Physical Address = %d\n", addr2);
    }
    
    // (iii) 1222 byte of segment 0
    printf("Segment 0, Offset 1222: ");
    int addr3 = logicalToPhysical(0, 1222);
    if (addr3 != -1) {
        printf("Physical Address = %d\n", addr3);
    }
    
    printf("\n");
}

// Interactive address translation
void interactiveMode() {
    int segment, offset;
    printf("=== INTERACTIVE ADDRESS TRANSLATION ===\n");
    printf("Enter segment number (0-4): ");
    scanf("%d", &segment);
    printf("Enter offset: ");
    scanf("%d", &offset);
    
    int physical = logicalToPhysical(segment, offset);
    if (physical != -1) {
        printf("Logical Address (Segment %d, Offset %d) → Physical Address %d\n", 
               segment, offset, physical);
    }
}

int main() {
    printf("SEGMENTED MEMORY ADDRESS TRANSLATION\n");
    printf("=====================================\n\n");
    
    // Display segment table
    printSegmentTable();
    
    // Test the required addresses
    testAddresses();
    
    // Interactive mode
    char choice;
    printf("Do you want to test another address? (y/n): ");
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y') {
        interactiveMode();
    }
    
    return 0;
}
