#include <signal.h>
#include <stdio.h>

void my_handler(int nsig) {
  if(nsig == SIGINT)
    printf("Receive signal %d, CTRL-C pressed\n", nsig);
  if(nsig == SIGQUIT)
    printf("Receive signal %d, CTRL-4 pressed \n", nsig);
}

int main(void) {
  //
  // Set the process response to the SIGINT signal
  //
  (void)signal(SIGINT, my_handler);
  (void)signal(SIGQUIT, my_handler);
  //
  // From this point, the process will print SIGINT message.
  //
  while(1);
  return 0;
}
