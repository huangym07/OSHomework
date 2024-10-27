#include <stdio.h>
#include <process.h>

int main(int argc, char *argv[]) {
	printf("hello!\n");
	for(int i = 0; i < MAX_PROCESS_NUMBER; i++) {
		printf("process pid is %d\n", process[i].pid);
	}
	return 0;
}
