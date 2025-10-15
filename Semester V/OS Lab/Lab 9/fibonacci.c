#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *generator(void *param) {
    int n = (long)param;
    int *arr = malloc(n * sizeof(int));

    if (arr == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }

    if (n > 0) arr[0] = 0;
    if (n > 1) arr[1] = 1;
    for (int i = 2; i < n; i++) {
        arr[i] = arr[i - 1] + arr[i - 2];
    }

    return (void*)arr;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_fibonacci_numbers>\n", argv[0]);
        exit(1);
    }

    int num_fibs = atoi(argv[1]);
    if (num_fibs <= 0) {
        fprintf(stderr, "Please enter a positive integer for the number of Fibonacci numbers.\n");
        exit(1);
    }

    pthread_t generate;
    void *ret;

    pthread_create(&generate, NULL, generator, (void*)(long)num_fibs);
    pthread_join(generate, &ret);

    int *fibonacci = (int*)ret;

    printf("Fibonacci Sequence: ");
    for (int i = 0; i < num_fibs; i++) {
        printf("%d ", fibonacci[i]);
    }
    printf("\n");

    free(fibonacci);

    return 0;
}
