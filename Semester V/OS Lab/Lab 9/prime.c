#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int is_prime(int num) {
    if (num <= 1) return 0;
    if (num == 2) return 1;
    if (num % 2 == 0) return 0;

    for (int i = 3; i*i <= num; i += 2) {
        if (num % i == 0) return 0;
    }

    return 1;
}

void *generate_primes(void *param) {
    int *range = (int*)param;
    int start = range[0];
    int end = range[1];

    printf("Prime numbers between %d and %d:\n", start, end);

    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            printf("%d ", i);
        }
    }
    printf("\n");

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <start_number> <end_number>\n", argv[0]);
        exit(1);
    }

    int start = atoi(argv[1]);
    int end = atoi(argv[2]);

    if (start < 0 || end < 0 || end < start) {
        fprintf(stderr, "Please enter valid positive integers where start <= end.\n");
        exit(1);
    }

    int range[2] = {start, end};

    pthread_t prime_thread;

    pthread_create(&prime_thread, NULL, generate_primes, (void*)range);
    pthread_join(prime_thread, NULL);

    return 0;
}
