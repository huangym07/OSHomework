#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>


int main() {
	if(fork() == 0) {
		printf("child process's pid is %d\n", getpid());

		char str[20];
		char s[3][20];
	
		for(int i = 0; i < 3; i++) {
			if(fscanf(stdin, "%s", s[i]) == 0) exit(1);
		}
		
		if(strcmp(s[0], s[1]) > 0) strcpy(str, s[0]);
		else strcpy(str, s[1]);
	
		if(strcmp(str, s[2]) < 0) strcpy(str, s[2]);
	
		int len = strlen(str);
		printf("The largest string is %s\nThe length is %d\n", str, len);

		exit(0);
	} else {
		printf("parent process's pid is %d\n", getpid());

		int rc = 0;
		wait(&rc);

		printf("return code is %d\nNow time is %ld\n", rc, time(NULL));
	}

	return 0;
}
