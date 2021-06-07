#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>

int num = 0;
int x = 0;
pid_t sender_pid = -1;

void handler_1(int sig) {
	if (sender_pid < 0) {
		return;
	}
	num |= 1 « x;

	printf(" ");
	x++;

	kill(sender_pid, SIGUSR1);
	if (x == 32) {
		printf("\nnum: %d\n", num);
		exit(0);
	}
}

void handler_0(int sig) {
	if (sender_pid < 0) {
		return;
	}

	printf(" ");
	x++;

	kill(sender_pid, SIGUSR1);
	if (x == 32) {
		printf("\nnum: %d\n", num);
		exit(0);
	}
}


int main() {
	pid_t pid;

	(void)signal(SIGUSR1, handler_1);
	(void)signal(SIGUSR2, handler_0);

	pid = getpid();
	printf("id: %d\n", (int)pid);

	printf("sender id: ");
	scanf("%d", &sender_pid);

	while (1);

	return 0;
}
