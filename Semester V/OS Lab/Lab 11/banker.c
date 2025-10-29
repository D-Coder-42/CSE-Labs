#include <stdio.h>
#include <stdbool.h>

#define R 3
int total[R] = {10, 5, 7};

void inputMatrix(int m[][R], int n, char *label) {
    for (int i = 0; i < n; i++) {
        printf("%s for P%d: ", label, i);
        for (int j = 0; j < R; j++) scanf("%d", &m[i][j]);
    }
}

void calculateNeed(int need[][R], int max[][R], int alloc[][R], int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < R; j++)
            need[i][j] = max[i][j] - alloc[i][j];
}

void calculateAvailable(int available[R], int alloc[][R], int n) {
    for (int j = 0; j < R; j++) {
        available[j] = total[j];
        for (int i = 0; i < n; i++)
            available[j] -= alloc[i][j];
    }
}

bool isSafe(int available[R], int alloc[][R], int need[][R], int n) {
    int work[R], finish[n], seq[n], count = 0;
    for (int i = 0; i < R; i++) work[i] = available[i];
    for (int i = 0; i < n; i++) finish[i] = 0;

    while (count < n) {
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                bool canRun = true;
                for (int j = 0; j < R; j++)
                    if (need[i][j] > work[j]) canRun = false;
                if (canRun) {
                    for (int j = 0; j < R; j++) work[j] += alloc[i][j];
                    seq[count++] = i;
                    finish[i] = 1;
                    found = true;
                }
            }
        }
        if (!found) return false;
    }

    printf("Safe sequence: ");
    for (int i = 0; i < n; i++) printf("P%d ", seq[i]);
    printf("\n");
    return true;
}

bool request(int pid, int req[R], int available[R], int max[][R], int alloc[][R], int n) {
    int need[n][R]; calculateNeed(need, max, alloc, n);
    for (int i = 0; i < R; i++) {
        if (req[i] > need[pid][i] || req[i] > available[i]) return false;
    }
    for (int i = 0; i < R; i++) {
        available[i] -= req[i];
        alloc[pid][i] += req[i];
        need[pid][i] -= req[i];
    }
    if (isSafe(available, alloc, need, n)) return true;
    for (int i = 0; i < R; i++) {
        available[i] += req[i];
        alloc[pid][i] -= req[i];
        need[pid][i] += req[i];
    }
    return false;
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    int alloc[n][R], max[n][R], need[n][R], available[R];
    inputMatrix(alloc, n, "Allocation");
    inputMatrix(max, n, "Maximum");
    calculateAvailable(available, alloc, n);
    calculateNeed(need, max, alloc, n);

    isSafe(available, alloc, need, n);

	printf("P1 request (1,0,2):\n");
    int req1[R] = {1, 0, 2};
    if (n > 1 && request(1, req1, available, max, alloc, n))
        printf("P1 request granted\n");
    else
        printf("P1 request denied\n");

    return 0;
}
