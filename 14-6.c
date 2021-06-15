#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>

void my_handler(int nsig) {
  int status[10];
  pid_t pid[10];

  for(int i = 0; i < 10; i++)
     pid[i] = wait(&status[i]);

  for(int i = 0; i < 10; i++){
  if (pid[i] < 0) {
    printf("Some error on waitpid errno = %d\n", errno);
  } else {
    if ((status[i] & 0xff) == 0) {
        printf("Process %d was exited with status %d\n", pid[i], status[i] >> 8);
    } else if ((status[i] & 0xff00) == 0) {
        printf("Process %d killed by signal %d %s\n", pid[i], status[i] &0x7f,
               (status[i] & 0x80) ? "with core file" : "without core file");
    }
  }
  }
  (void) signal(SIGCHLD, SIG_IGN);
}

int main(void) {
  pid_t pid;
  int i,j;

  (void) signal(SIGCHLD, my_handler);

  for (i=0; i<10; i++) {
    if ((pid = fork()) < 0) {
      printf("Can\'t fork child 1\n");
      exit(1);
    } else if (pid == 0) {
      /* Child  */
      for (j=1; j < 10000000; j++);
      exit(200 + i);
    }
  }

  while(1);
  return 0;
}
