#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>

int num = 0;
int x = 0;
pid_t recipient_pid;

void send() {
	int bit = 1 « x;
	if (num & bit) {
		printf(" ");
		kill(recipient_pid, SIGUSR1);
	}
	else {
		printf(" ");
		kill(recipient_pid, SIGUSR2);
	}
	x++;
}

void ok(int sig) {
	if (x == 32) {
		printf("ok");
		exit(0);
	}
	else {
		printf(" ");
		send();
	}
}


int main() {
	pid_t pid;

	pid = getpid();
	printf("id: %d\n", (int)pid);

	printf("recipient id: ");
	scanf("%d", &recipient_pid);

	printf("num: ");
	scanf("%d", &num);

	(void)signal(SIGUSR1, ok);

	send();

	while (1);

	return 0;
}
