#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <errno.h>

/**
 *  Server
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
    
    //https://stackoverflow.com/questions/5339200/how-to-create-a-single-instance-application-in-c-or-c
    // Вешаем лок на файл, если лок есть - значит сервер работает.
    int pidFile = open("11-3b.c", O_CREAT | O_RDWR, 0666);
    int rc = flock(pidFile, LOCK_EX | LOCK_NB);
    if(!rc){
        if ((key = ftok(pathname,0)) < 0) {
            printf("Can\'t generate key\n");
            exit(-1);
        }

        if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
            printf("Can\'t get msqid\n");
            exit(-1);
        }

        while(1){
            if (msgrcv(msqid, (struct clientbuf *) &clientbuf, sizeof(clientbuf.info), -2, 0) < 0) {
                printf("Can\'t receive message from queue\n");
                exit(-1);
            }

            if(clientbuf.mtype == 2){
                printf("Меня закиллили\n");
                exit(-1);
            }

            printf("Клиент pid = %d прислал: %f\n", clientbuf.info.pid, clientbuf.info.message);

            serverbuf.mtype = clientbuf.info.pid;
            serverbuf.info.message = clientbuf.info.message * clientbuf.info.message;

            if (msgsnd(msqid, &serverbuf, sizeof(serverbuf.info), 0) < 0) {
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
            }
            
            printf("Выслали ответ клиенту.\n");
            
        }
    }
    else{
        printf("Сервер уже запущен.\n");
    }
  return 0;
}
