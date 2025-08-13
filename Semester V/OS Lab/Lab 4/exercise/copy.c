#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: ./copy <source> <destination>\n");
        return 1;
    }

    FILE *src = fopen(argv[1], "r");
    if (!src) {
    	perror("fopen"); 
    	exit(EXIT_FAILURE); 
    }

    FILE *dest = fopen(argv[2], "w");
    if (!dest) { 
    	perror("fopen"); 
    	fclose(src); 
    	exit(EXIT_FAILURE); 
    }

    int ch;
    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dest);
    }

    fclose(src);
    fclose(dest);
    return 0;
}

