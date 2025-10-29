#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define R 3
int total[R] = {10, 5, 7};
int *available, **maximum, **allocation, **need;
int n;

pthread_mutex_t lock;

void calculateNeed() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < R; j++)
            need[i][j] = maximum[i][j] - allocation[i][j];
}

bool isSafe() {
    int work[R], finish[n], count = 0;
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
                    for (int j = 0; j < R; j++) work[j] += allocation[i][j];
                    finish[i] = 1;
                    count++;
                    found = true;
                }
            }
        }
        if (!found) return false;
    }
    return true;
}

bool requestResources(int pid, int req[R]) {
    pthread_mutex_lock(&lock);
    for (int i = 0; i < R; i++) {
        if (req[i] > need[pid][i] || req[i] > available[i]) {
            pthread_mutex_unlock(&lock);
            return false;
        }
    }
    for (int i = 0; i < R; i++) {
        available[i] -= req[i];
        allocation[pid][i] += req[i];
        need[pid][i] -= req[i];
    }
    bool safe = isSafe();
    if (!safe) {
        for (int i = 0; i < R; i++) {
            available[i] += req[i];
            allocation[pid][i] -= req[i];
            need[pid][i] += req[i];
        }
    }
    pthread_mutex_unlock(&lock);
    return safe;
}

void releaseResources(int pid) {
    pthread_mutex_lock(&lock);
    for (int i = 0; i < R; i++) {
        available[i] += allocation[pid][i];
        allocation[pid][i] = 0;
        need[pid][i] = maximum[pid][i];
    }
    pthread_mutex_unlock(&lock);
}

void *process(void *arg) {
    int pid = *(int *)arg;
    int req[R];
    for (int t = 0; t < 3; t++) {
        for (int i = 0; i < R; i++)
            req[i] = rand() % (need[pid][i] + 1);
        if (requestResources(pid, req)) {
            printf("P%d: Request granted\n", pid);
            sleep(1);
            releaseResources(pid);
            printf("P%d: Resources released\n", pid);
        } else {
            printf("P%d: Request denied\n", pid);
        }
        sleep(1);
    }
    return NULL;
}

int main() {
    printf("Enter number of processes: ");
    scanf("%d", &n);

    available = malloc(R * sizeof(int));
    for (int i = 0; i < R; i++) available[i] = total[i];

    maximum = malloc(n * sizeof(int *));
    allocation = malloc(n * sizeof(int *));
    need = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        maximum[i] = malloc(R * sizeof(int));
        allocation[i] = malloc(R * sizeof(int));
        need[i] = malloc(R * sizeof(int));
        printf("Max for P%d: ", i);
        for (int j = 0; j < R; j++) scanf("%d", &maximum[i][j]);
        printf("Alloc for P%d: ", i);
        for (int j = 0; j < R; j++) {
            scanf("%d", &allocation[i][j]);
            available[j] -= allocation[i][j];
        }
    }

    calculateNeed();
    pthread_mutex_init(&lock, NULL);

    pthread_t threads[n];
    int ids[n];
    for (int i = 0; i < n; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, process, &ids[i]);
    }
    for (int i = 0; i < n; i++)
        pthread_join(threads[i], NULL);

    pthread_mutex_destroy(&lock);
    return 0;
}

/*
void *process(void *arg) {
    int pid = *(int *)arg;
    int req[R];
    for (int t = 0; t < 3; t++) {
        for (int i = 0; i < R; i++)
            req[i] = rand() % (need[pid][i] + 1);
        if (requestResources(pid, req)) {
            printf("P%d: Request granted\n", pid);
            sleep(1);
            releaseResources(pid);
            printf("P%d: Resources released\n", pid);
        } else {
            printf("P%d: Request denied\n", pid);
        }
        sleep(1);
    }
    return NULL;
}

int main() {
    printf("Enter number of processes: ");
    scanf("%d", &n);

    available = malloc(R * sizeof(int));
    for (int i = 0; i < R; i++) available[i] = total[i];

    maximum = malloc(n * sizeof(int *));
    allocation = malloc(n * sizeof(int *));
    need = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        maximum[i] = malloc(R * sizeof(int));
        allocation[i] = malloc(R * sizeof(int));
        need[i] = malloc(R * sizeof(int));
        printf("Max for P%d: ", i);
        for (int j = 0; j < R; j++) scanf("%d", &maximum[i][j]);
        printf("Alloc for P%d: ", i);
        for (int j = 0; j < R; j++) {
            scanf("%d", &allocation[i][j]);
            available[j] -= allocation[i][j];
        }
    }

    calculateNeed(n, need, maximum, allocation);
    pthread_mutex_init(&lock, NULL);

    pthread_t threads[n];
    int ids[n];
    for (int i = 0; i < n; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, process, &ids[i]);
    }
    for (int i = 0; i < n; i++)
        pthread_join(threads[i], NULL);

    pthread_mutex_destroy(&lock);
    return 0;
}
*/
