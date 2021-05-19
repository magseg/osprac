#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/**
 *  Client
 */
struct client
{
    long mtype;
    struct
    {
        pid_t pid;
        float message;
    } info;
} clientbuf;

struct server
{
    long mtype;
    struct{
    float message;
    } info;
} serverbuf;


int main(void)
{
  int msqid;
  char pathname[]="11-3b.c";
  key_t  key;
  int len, maxlen;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

// Client initialization
clientbuf.mtype = 1;
clientbuf.info.pid = getpid();

float msg;
printf("Please enter a floating point number: ");
scanf("%f", &msg);
clientbuf.info.message = msg;

printf("Client send with type = %d message = '%f'\n", clientbuf.mtype, clientbuf.info.message);

len = sizeof(clientbuf.info);
if (msgsnd(msqid, &clientbuf, len, 0) < 0) {
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    exit(-1);
}

printf("Waiting server response...\n");

if (msgrcv(msqid, &serverbuf, sizeof(serverbuf.info), getpid(), 0) < 0) {
    printf("Can\'t receive message from queue\n");
    exit(-1);
}
printf("Server response: %f\n", serverbuf.info.message);

  
  return 0;
}
