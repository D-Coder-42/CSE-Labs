#include <stdio.h>

#define MAX 100
#define Q0_QUANTUM 2
#define MAX_SJF_TIME 5

typedef struct {
    int pid, arrival, burst, remaining;
    int start, complete, wait, turn;
    int queue;
} Process;

void inputProcesses(Process p[], int n);
void enqueueNewArrivals(Process p[], int n, int time, int q0[], int *rear0);
int getNextQueue(int rear[], int front[], int sjf_time_used);
int selectProcess(Process p[], int q[], int *front, int *rear, int queue);
int executeProcess(Process *current, int *time, int *slice, int *sjf_time_used);
void demoteProcess(Process *current, Process p[], int q1[], int *rear1, int q2[], int *rear2);
void printResults(Process p[], int n);

int main() {
    int n, time = 0, done = 0, sjf_time_used = 0;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process p[n];
    inputProcesses(p, n);

    int q0[MAX], q1[MAX], q2[MAX];
    int front[3] = {0}, rear[3] = {0};
    Process *current = NULL;
    int slice = 0;

    while (done < n) {
        enqueueNewArrivals(p, n, time, q0, &rear[0]);

        int activeQ = getNextQueue(rear, front, sjf_time_used);

        if (current && current->queue > activeQ && activeQ != -1) {
            printf("Preempting PID %d from Q%d to Q%d\n", current->pid, current->queue, activeQ);
            demoteProcess(current, p, q1, &rear[1], q2, &rear[2]);
            current = NULL;
            slice = 0;
        }

        if (!current && activeQ != -1) {
            int idx = selectProcess(p, (activeQ == 0 ? q0 : activeQ == 1 ? q1 : q2), &front[activeQ], &rear[activeQ], activeQ);
            current = &p[idx];
            if (current->start == -1) current->start = time;
            slice = 0;
            printf("Starting PID %d from Q%d at time %d\n", current->pid, current->queue, time);
        }

        if (current) {
            if (executeProcess(current, &time, &slice, &sjf_time_used)) {
                current = NULL;
                done++;
                slice = 0;
            } else if (current->queue == 0 && slice == Q0_QUANTUM) {
                printf("Time slice expired for PID %d in Q0\n", current->pid);
                current->queue = 1;
                q1[rear[1]++] = current - p;
                current = NULL;
                slice = 0;
            }
        } else {
            printf("CPU idle at time %d\n", time);
            time++;
        }
    }

    printResults(p, n);
    return 0;
}

void inputProcesses(Process p[], int n) {
    for (int i = 0; i < n; i++) {
        printf("Enter PID, Arrival, Burst for P%d: ", i + 1);
        scanf("%d %d %d", &p[i].pid, &p[i].arrival, &p[i].burst);
        p[i].remaining = p[i].burst;
        p[i].start = -1;
        p[i].queue = 0;
    }
}

void enqueueNewArrivals(Process p[], int n, int time, int q0[], int *rear0) {
    for (int i = 0; i < n; i++) {
        if (p[i].arrival == time && p[i].remaining > 0 && p[i].queue == 0) {
            q0[(*rear0)++] = i;
            printf("Enqueued PID %d to Q0 at time %d\n", p[i].pid, time);
        }
    }
}

int getNextQueue(int rear[], int front[], int sjf_time_used) {
    for (int i = 0; i < 3; i++) {
        if (i == 1 && sjf_time_used >= MAX_SJF_TIME) continue;
        if (rear[i] > front[i]) return i;
    }
    return -1;
}

int selectProcess(Process p[], int q[], int *front, int *rear, int queue) {
    if (queue == 1) {
        int min = 1e9, pos = -1;
        for (int i = *front; i < *rear; i++) {
            if (p[q[i]].remaining < min) {
                min = p[q[i]].remaining;
                pos = i;
            }
        }
        int idx = q[pos];
        for (int i = pos; i < *rear - 1; i++) q[i] = q[i + 1];
        (*rear)--;
        return idx;
    } else {
        return q[(*front)++];
    }
}

int executeProcess(Process *current, int *time, int *slice, int *sjf_time_used) {
    current->remaining--;
    (*slice)++;
    (*time)++;
    if (current->queue == 1) (*sjf_time_used)++;
    printf("Executing PID %d at time %d, remaining: %d\n", current->pid, *time, current->remaining);

    if (current->remaining == 0) {
        current->complete = *time;
        current->turn = current->complete - current->arrival;
        current->wait = current->turn - current->burst;
        printf("PID %d completed at time %d\n", current->pid, *time);
        return 1;
    }
    return 0;
}

void demoteProcess(Process *current, Process p[], int q1[], int *rear1, int q2[], int *rear2) {
    if (current->queue == 1) q1[(*rear1)++] = current - p;
    else q2[(*rear2)++] = current - p;
}

void printResults(Process p[], int n) {
    printf("\nPID\tArrival\tBurst\tStart\tComplete\tWait\tTurnaround\n");
    float total_wait = 0, total_turn = 0;
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t\t%d\t%d\n",
               p[i].pid, p[i].arrival, p[i].burst,
               p[i].start, p[i].complete, p[i].wait, p[i].turn);
        total_wait += p[i].wait;
        total_turn += p[i].turn;
    }
    printf("\nAverage Waiting Time: %.2f\n", total_wait / n);
    printf("Average Turnaround Time: %.2f\n", total_turn / n);
}
