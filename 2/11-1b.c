#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  int msqid;
  char pathname[]="11-1a.c";
  key_t  key;
  int len, maxlen, i;

  struct mymsgbuf
  {
    long mtype;
    struct{
           int end;
    }inf;
    char mtext[1];
  } mybuf;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  while (1) {
    //
    // In an infinite loop, accept messages of any type in FIFO order
    // with a maximum length of 81 characters
    // until a message of type LAST_MESSAGE is received.
    //
    maxlen = 20;

    if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 1, 0)) < 0) {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }
    //
    // If the received message is of type LAST_MESSAGE,
    // then terminate and remove the message queue from the system.
    // Otherwise, print the text of the received message.
    //
    if (mybuf.inf.end == 1) {
      break;
    }

    printf("message type = %ld, info = %s\n", mybuf.mtype, mybuf.mtext);
  }

  for (i = 1; i <= 5; i++) {

    mybuf.mtype = 2;
    mybuf.inf.end = 0;
    strcpy(mybuf.mtext, "B");
    len = strlen(mybuf.mtext)+1+sizeof(mybuf.inf);

    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }
  }  

  mybuf.mtype = 2;
  mybuf.inf.end = 1;
  len = sizeof(mybuf.inf) + 1;

  if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    exit(-1);
  }

  return 0;
}
