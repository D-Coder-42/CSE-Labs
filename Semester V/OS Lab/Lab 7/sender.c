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

int main () {
	int running = 1;
	int msgid;
	msg data;
	char buffer[MAXSIZE];
	msgid = msgget((key_t) 1234, 0666 | IPC_CREAT);
	if (msgid == -1) {
		perror("msgget() failed");
		exit(EXIT_FAILURE);
	}
	
	while(running) {
		printf("Enter some text: ");
		fgets(buffer, MAXSIZE, stdin);
		data.msg_type = 1;
		strcpy(data.payload, buffer);
		if (msgsnd(msgid, (void*) &data, MAXSIZE, 0) == -1) {
			perror("msgsnd() failed");
			exit(EXIT_FAILURE);
		}
		if (strncmp(data.payload, "end", 3) == 0) {
			running = 0;
		}
	}
	exit(EXIT_SUCCESS);
}
		
