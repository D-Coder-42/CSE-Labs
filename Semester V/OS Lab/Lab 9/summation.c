#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *calculator(void *param) {
    int *arr = (int*)param;
    int length = arr[0];
    int *sum = malloc(sizeof(int));

    if (sum == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }

    *sum = 0;

    for (int i = 1; i <= length; i++) {
        if (arr[i] >= 0) {
            *sum += arr[i];
        }
    }

    return (void*)sum;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <integer1> <integer2> ... <integerN>\n", argv[0]);
        exit(1);
    }

    int length = argc - 1;
    int *arr = malloc((length + 1) * sizeof(int));

    if (arr == NULL) {
        perror("Failed to allocate memory for the array");
        exit(1);
    }

    arr[0] = length;
    for (int i = 1; i <= length; i++) {
        arr[i] = atoi(argv[i]);
    }

    pthread_t calc_thread;
    void *ret;

    pthread_create(&calc_thread, NULL, calculator, (void*)arr);
    pthread_join(calc_thread, &ret);
    int *sum_result = (int*)ret;

    printf("Sum of non-negative integers: %d\n", *sum_result);
    free(arr);
    free(sum_result);

    return 0;
}
