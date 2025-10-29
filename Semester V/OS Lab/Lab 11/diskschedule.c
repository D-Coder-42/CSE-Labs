#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

typedef struct DSA {
    int request_id; 
    int arrival_time_stamp;
    int cylinder;
    int address;
    int process_id;
} DSA;

void fcfs(DSA requests[], int n, int start_cylinder) {
    int total_seek_time = 0;
    int current = start_cylinder;

    printf("\nFCFS Disk Scheduling Order:\n");
    for (int i = 0; i < n; i++) {
        printf("Request %d at cylinder %d\n", requests[i].request_id, requests[i].cylinder);
        total_seek_time += abs(current - requests[i].cylinder);
        current = requests[i].cylinder;
    }

    printf("Total Seek Time: %d\n", total_seek_time);
    printf("Average Seek Time: %.2f\n", (float)total_seek_time / n);
}

void sstf(DSA requests[], int n, int start_cylinder) {
    int total_seek_time = 0;
    int current = start_cylinder;
    int completed[n];
    for (int i = 0; i < n; i++) completed[i] = 0;

    printf("\nSSTF Disk Scheduling Order:\n");
    for (int count = 0; count < n; count++) {
        int min_distance = INT_MAX;
        int index = -1;

        for (int i = 0; i < n; i++) {
            if (!completed[i]) {
                int distance = abs(current - requests[i].cylinder);
                if (distance < min_distance) {
                    min_distance = distance;
                    index = i;
                }
            }
        }

        if (index != -1) {
            printf("Request %d at cylinder %d\n", requests[index].request_id, requests[index].cylinder);
            total_seek_time += abs(current - requests[index].cylinder);
            current = requests[index].cylinder;
            completed[index] = 1;
        }
    }

    printf("Total Seek Time: %d\n", total_seek_time);
    printf("Average Seek Time: %.2f\n", (float)total_seek_time / n);
}

int main() {
    DSA requests[] = {
        {1, 0, 98, 0, 1},
        {2, 1, 183, 0, 2},
        {3, 2, 37, 0, 3},
        {4, 3, 122, 0, 4},
        {5, 4, 14, 0, 5}
    };
    int n = sizeof(requests) / sizeof(requests[0]);
    int start_cylinder = 53;

    fcfs(requests, n, start_cylinder);
    sstf(requests, n, start_cylinder);

    return 0;
}
