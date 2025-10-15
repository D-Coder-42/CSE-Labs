#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *sum_even(void *param) {
    int *arr = (int *)param;
    int size = arr[0];
    int *sum = malloc(sizeof(int));
    *sum = 0;

    for (int i = 1; i <= size; i++) {
        if (arr[i] % 2 == 0) {
            *sum += arr[i];
        }
    }

    return sum;
}

void *sum_odd(void *param) {
    int *arr = (int *)param;
    int size = arr[0];
    int *sum = malloc(sizeof(int));
    *sum = 0;

    for (int i = 1; i <= size; i++) {
        if (arr[i] % 2 != 0) {
            *sum += arr[i];
        }
    }

    return sum;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <number1> <number2> ... <numberN>\n", argv[0]);
        exit(1);
    }

    int size = argc - 1;
    int *arr = malloc((size + 1) * sizeof(int));
    if (arr == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }

    arr[0] = size;
    for (int i = 1; i <= size; i++) {
        arr[i] = atoi(argv[i]);
    }

    pthread_t even_thread, odd_thread;

    pthread_create(&even_thread, NULL, sum_even, (void *)arr);
    pthread_create(&odd_thread, NULL, sum_odd, (void *)arr);

    int *even_result, *odd_result;

    pthread_join(even_thread, (void **)&even_result);
    pthread_join(odd_thread, (void **)&odd_result);

    printf("Sum of even numbers: %d\n", *even_result);
    printf("Sum of odd numbers: %d\n", *odd_result);

    free(even_result);
    free(odd_result);
    free(arr);

    return 0;
}
