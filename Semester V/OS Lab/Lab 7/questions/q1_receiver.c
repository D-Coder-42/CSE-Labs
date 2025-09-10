#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/*
 * int msgget(key_t key, int msgflg);
 * int msgsnd(int msqid, const void *msg_ptr, size_t msg_sz, int msgflg);
 * int msgrcv(int msqid, void *msg_ptr, size_t msg_sz, long int msgtype, int msgflg);
 * int msgctl(int msqid, int command, struct msqid_ds *buf);
 * 
 * struct msqid_ds {
 * 	uid_t msg_perm.uid;
 * 	uid_t msg_perm.gid;
 * 	mode_t msg_perm.mode;
 * };
 * */
 
#define MAXSIZE 512

typedef struct {
	long int msg_type;
	char payload[MAXSIZE];
} msg;

int palindrome(int num) {
	int og = num;
	int rev = 0;
	
	while (og > 0) {
		rev = rev*10 + og%10;
		og /= 10;
	}
	
	if (rev == num) return 1;
	return 0;
}

int main () {
	int running = 1;
	int msgid;
	msg data;
	long int recv_msg = 0;
	msgid = msgget((key_t) 1234, 0666 | IPC_CREAT);
	if (msgid == -1) {
		perror("msgget() failed");
		exit(EXIT_FAILURE);
	}
	
	while(running) {
		if (msgrcv(msgid, (void*) &data, MAXSIZE, recv_msg, 0) == -1) {
			perror("msgrcv() failed");
			exit(EXIT_FAILURE);
		}
		if (strncmp(data.payload, "end", 3) == 0) {
			running = 0;
			break;
		}
		
		int num = atoi(data.payload);
		if (palindrome(num)) {
			printf("%d is a palindrome\n", num);
		} else {
			printf("%d is not a palindrome\n", num);
		}
	}
	
	if (msgctl(msgid, IPC_RMID, 0) == -1) {
		perror("msgctl(IPC_RMID) failed");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
		
