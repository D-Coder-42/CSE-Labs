#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Memory allocation block structure
struct mab {
    int offset;
    int size;
    int allocated;
    struct mab *next;
    struct mab *prev;
};
typedef struct mab Mab;
typedef Mab *MabPtr;

// Function prototypes
MabPtr memChk(MabPtr m, int size, char policy);
MabPtr memAlloc(MabPtr m, int size, char policy);
MabPtr memFree(MabPtr m);
MabPtr memMerge(MabPtr m);
MabPtr memSplit(MabPtr m, int size);
MabPtr createInitialBlock(int total_size);
MabPtr getHead(MabPtr any);
void printMemory(MabPtr head);
void freeMemoryList(MabPtr head);

// Policy types
#define FIRST_FIT 'F'
#define BEST_FIT  'B'
#define WORST_FIT 'W'
#define NEXT_FIT  'N'

// Example usage and test
int main() {
    // Create 1KB memory pool
    MabPtr memory = createInitialBlock(1024);
    
    printMemory(memory);
    
    // Allocate using different policies
    printf("=== FIRST FIT ALLOCATIONS ===\n");
    memory = memAlloc(memory, 200, FIRST_FIT);
    memory = memAlloc(memory, 300, FIRST_FIT);
    memory = memAlloc(memory, 150, FIRST_FIT);
    printMemory(memory);
    
    // To free a specific block, you need its pointer. For demo, free the first allocated (head)
    printf("=== FREEING BLOCKS ===\n");
    memory = freeByOffset(memory,0);  // Frees the head block (assuming it's allocated)
    memory = freeByOffset(memory,200);
    printMemory(memory);
    
    printf("=== BEST FIT ALLOCATION ===\n");
    memory = memAlloc(memory, 300, BEST_FIT);
    printMemory(memory);
    
    // Clean up
    freeMemoryList(memory);
    return 0;
}

// Helper to get the head of the list from any block
MabPtr getHead(MabPtr any) {
    if (!any) return NULL;
    while (any->prev) {
        any = any->prev;
    }
    return any;
}

// Find suitable block based on policy
MabPtr memChk(MabPtr m, int size, char policy) {
    if (!m || size <= 0) return NULL;
    
    MabPtr best = NULL;
    int best_fit = (policy == WORST_FIT) ? 0 : 999999;
    MabPtr current = m;
    MabPtr next_start = NULL;
    
    do {
        if (!current->allocated && current->size >= size) {
            int fit_score = current->size - size;
            
            switch (policy) {
                case FIRST_FIT:
                    return current;  // Return first match
                case BEST_FIT:
                    if (fit_score < best_fit) {
                        best_fit = fit_score;
                        best = current;
                    }
                    break;
                case WORST_FIT:
                    if (fit_score > best_fit) {
                        best_fit = fit_score;
                        best = current;
                    }
                    break;
                case NEXT_FIT:
                    if (!next_start) next_start = current;
                    return current;  // Simplified; external tracking recommended for full Next Fit
            }
        }
        current = current->next;
    } while (current);
    
    return best;
}

// Allocate memory using specified policy
MabPtr memAlloc(MabPtr head, int size, char policy) {
    if (!head || size <= 0) return head;
    
    MabPtr block = memChk(head, size, policy);
    if (!block) return head;  // No suitable block found
    
    // Split if necessary
    if (block->size > size) {
        memSplit(block, size);
    } else {
        block->allocated = 1;
    }
    
    return head;
}

// Split a block into two: allocated part and remaining free part
MabPtr memSplit(MabPtr block, int size) {
    if (!block || block->allocated || block->size <= size) return block;
    
    // Create new free block for remainder
    MabPtr new_block = (MabPtr)malloc(sizeof(Mab));
    new_block->offset = block->offset + size;
    new_block->size = block->size - size;
    new_block->allocated = 0;
    
    // Update original block
    block->size = size;
    block->allocated = 1;
    
    // Insert new block after original
    new_block->next = block->next;
    new_block->prev = block;
    if (block->next) block->next->prev = new_block;
    block->next = new_block;
    
    return block;
}

// Merge a free block with adjacent free blocks
MabPtr memMerge(MabPtr m) {
    if (!m || m->allocated) return m;
    
    // Merge with previous if free
    if (m->prev && !m->prev->allocated) {
        m->prev->size += m->size;
        m->prev->next = m->next;
        if (m->next) m->next->prev = m->prev;
        MabPtr temp = m;
        m = m->prev;
        free(temp);
    }
    
    // Merge with next if free
    if (m->next && !m->next->allocated) {
        m->size += m->next->size;
        MabPtr temp = m->next;
        m->next = temp->next;
        if (m->next) m->next->prev = m;
        free(temp);
    }
    
    return m;
}

// Free a memory block (set to free and then merge)
MabPtr memFree(MabPtr block) {
    if (!block || !block->allocated) return getHead(block);
    
    MabPtr head = getHead(block);
    block->allocated = 0;
    memMerge(block);
    return head;
}

// Find and free block at specific offset
MabPtr freeByOffset(MabPtr head, int target_offset) {
    if (!head) return NULL;
    
    MabPtr current = head;
    while (current) {
        if (current->offset == target_offset && current->allocated) {
            return memFree(current);  // Returns updated head
        }
        current = current->next;
    }
    printf("No allocated block found at offset %d\n", target_offset);
    return head;
}

// Create initial memory block
MabPtr createInitialBlock(int total_size) {
    MabPtr head = (MabPtr)malloc(sizeof(Mab));
    head->offset = 0;
    head->size = total_size;
    head->allocated = 0;
    head->next = NULL;
    head->prev = NULL;
    return head;
}

// Print memory layout
void printMemory(MabPtr head) {
    printf("Memory Layout:\n");
    printf("Offset\tSize\tStatus\n");
    printf("------\t----\t------\n");
    
    MabPtr current = head;
    while (current) {
        printf("%d\t%d\t%s\n", 
               current->offset, 
               current->size, 
               current->allocated ? "ALLOCATED" : "FREE");
        current = current->next;
    }
    printf("\n");
}

// Free entire memory list
void freeMemoryList(MabPtr head) {
    MabPtr current = head;
    while (current) {
        MabPtr next = current->next;
        free(current);
        current = next;
    }
}
