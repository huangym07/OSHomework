#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	char str[20];
	char s[3][20];

	for(int i = 0; i < 3; i++) {
		gets(s[i]);
	}
	
	if(strcmp(s[0], s[1]) > 0) strcpy(str, s[0]);
	else strcpy(str, s[1]);

	if(strcmp(str, s[2]) < 0) strcpy(str, s[2]);

	int len = strlen(str);
	printf("\nThe largest string is %s\nThe length is %d\n", str, len);

	return 0;
}
