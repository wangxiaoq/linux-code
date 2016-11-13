#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "user-log"

int main(void)
{
	char* home = getenv("HOME");
	FILE* fp = NULL;
	char file_path[128];

	strcpy(file_path, home);
	strcat(file_path, "/");
	strcat(file_path, FILENAME);

	fp = fopen(file_path, "w+");
	if (fp == NULL) {
		perror("Cannot open for write: ");
		exit(-1);
	}
	fprintf(fp, "hello userspace!\n");
	fclose(fp);

	return 0;
}
