#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_LEN 256  // Size of the alphabet (256 for extended ASCII characters)
#define max(a, b) ((a < b) ? b : a)  // Macro to get the maximum of two numbers

int chars_compared;  // Variable to track the number of characters compared during the search

// delta1 table: 
// delta1[c] contains the shift distance for character 'c' when a mismatch occurs at that position.
// If 'c' does not occur in the pattern, then delta1[c] = pattern length (meaning shift by the full length).
// If 'c' is found in the pattern, the value in delta1 tells how far to shift the pattern.
void make_delta1(int *delta1, uint8_t *pat, int32_t patlen) {
    int i;

    // Initialize delta1: set all entries to the length of the pattern
    for (i = 0; i < ALPHABET_LEN; i++) {
        delta1[i] = patlen;
    }

    // Fill delta1 for characters that are present in the pattern
    for (i = 0; i < patlen - 1; i++) {
        delta1[pat[i]] = patlen - 1 - i;
    }
}

// is_prefix function: 
// This function checks if the suffix starting at position 'pos' in the word is a prefix of the word.
int is_prefix(uint8_t *word, int wordlen, int pos) {
    int i;
    int suffixlen = wordlen - pos;

    // Compare the suffix starting at 'pos' with the prefix
    for (i = 0; i < suffixlen; i++) {
        if (word[i] != word[pos + i]) {
            return 0;  // Suffix does not match the prefix
        }
    }
    return 1;  // Suffix is a prefix
}

// suffix_length function:
// This function returns the length of the longest suffix starting at position 'pos' that matches the suffix of the word.
// For example, for "dddbcabc", suffix_length("dddbcabc", 8, 4) returns 2
int suffix_length(uint8_t *word, int wordlen, int pos) {
    int i;
    // Compare characters from the position 'pos' backwards to find the longest matching suffix
    for (i = 0; (word[pos - i] == word[wordlen - 1 - i]) && (i < pos); i++);
    return i;
}

// delta2 table:
// delta2[p] helps in shifting the pattern when a mismatch occurs at position 'p' in the pattern.
// It considers both the longest prefix of the pattern that ends at position 'p', and the longest suffix of the pattern that ends at 'p'.
void make_delta2(int *delta2, uint8_t *pat, int32_t patlen) {
    int p;
    int last_prefix_index = 1;

    // First loop: handle prefixes
    for (p = patlen - 1; p >= 0; p--) {
        if (is_prefix(pat, patlen, p + 1)) {
            last_prefix_index = p + 1;
        }
        delta2[p] = (patlen - 1 - p) + last_prefix_index;
    }

    // Second loop: handle suffixes
    for (p = 0; p < patlen - 1; p++) {
        int slen = suffix_length(pat, patlen, p);
        if (pat[p - slen] != pat[patlen - 1 - slen]) {
            delta2[patlen - 1 - slen] = patlen - 1 - p + slen;
        }
    }
}

// boyer_moore function:
// This is the implementation of the Boyer-Moore string matching algorithm. It finds the first occurrence of a pattern (pat) 
// in the string (string) by using two tables (delta1 and delta2) to decide how to shift the pattern during mismatches.
uint32_t boyer_moore(uint8_t *string, uint32_t stringlen, uint8_t *pat, uint32_t patlen) {
    int i;
    int delta1[ALPHABET_LEN];  // Table for character shifts
    int *delta2 = malloc(patlen * sizeof(int));  // Table for suffix shifts
    
    make_delta1(delta1, pat, patlen);  // Initialize delta1 table
    make_delta2(delta2, pat, patlen);  // Initialize delta2 table

    int n_shifts = 0;  // Counter for number of shifts
    chars_compared = 0;  // Counter for number of character comparisons

    i = patlen - 1;  // Start from the end of the pattern
    while (i < stringlen) {  // Search until the entire string has been processed
        int j = patlen - 1;  // Start comparing from the end of the pattern
        while (j >= 0 && (string[i] == pat[j])) {
            --i;  // Move one character left in the string
            --j;  // Move one character left in the pattern
            chars_compared++;  // Count this comparison
        }

        if (j < 0) {
            free(delta2);  // Free dynamically allocated memory
            return (uint32_t) i + 1;  // Pattern found, return position
        }

        chars_compared++;  // Count this comparison
        i += max(delta1[string[i]], delta2[j]);  // Shift based on the delta tables
    }

    free(delta2);  // Free dynamically allocated memory
    return 0;  // Pattern not found
}

int main() {
    char input_string[1024], input_pattern[1024];

    // Get user input for the string and the pattern to search for
    printf("Enter the string to search in: ");
    fgets(input_string, sizeof(input_string), stdin);
    input_string[strcspn(input_string, "\n")] = 0;  // Remove the trailing newline character

    printf("Enter the pattern to search for: ");
    fgets(input_pattern, sizeof(input_pattern), stdin);
    input_pattern[strcspn(input_pattern, "\n")] = 0;  // Remove the trailing newline character

    // Perform the Boyer-Moore string search
    uint32_t pos = boyer_moore((uint8_t *)input_string, strlen(input_string), (uint8_t *)input_pattern, strlen(input_pattern));

    // Output the result
    if (pos == 0 && chars_compared != strlen(input_pattern))
        printf("Not Found - ");  // Pattern not found
    else
        printf("Found at position %u - ", pos);  // Pattern found at position
    printf("%d chars compared.\n", chars_compared);  // Output the number of character comparisons

    return 0;
}
