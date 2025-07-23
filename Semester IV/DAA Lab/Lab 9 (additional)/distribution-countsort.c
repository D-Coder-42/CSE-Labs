#include <stdio.h>
#include <string.h>
#define RANGE 256

void distributionCountingSort(char arr[]) {
    char output[strlen(arr)];
    int count[RANGE], i;

    memset(count, 0, sizeof(count));

    for(i = 0; arr[i]; ++i)
        ++count[arr[i]];

    for (i = 1; i < RANGE; ++i)
        count[i] += count[i - 1];

    for (i = 0; arr[i]; ++i) {
        output[count[arr[i]] - 1] = arr[i];
        --count[arr[i]];
    }

    for (i = 0; arr[i]; ++i)
        arr[i] = output[i];
}

int main() {
    char arr[1024];
    
    printf("Enter a string: ");
    fgets(arr, sizeof(arr), stdin);
    arr[strcspn(arr, "\n")] = 0;

    distributionCountingSort(arr);

    printf("Sorted character array is %s\n", arr);
    return 0;
}