#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define MODULE_NAME "/dev/vma-test"
#define PAGE_SIZE 4096

#define FATAL(s) do { \
	fprintf(stderr, "%s\n", s); \
	exit(-1); \
} while(0)

int main(void)
{
	char buf[PAGE_SIZE] = {'\0'};
	int fd = open(MODULE_NAME, O_RDWR);
	char *addr = mmap(NULL, PAGE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	memset(addr, 'a', PAGE_SIZE);
	if (read(fd, buf, PAGE_SIZE) < 0)
		FATAL("read file error");

	buf[PAGE_SIZE-1] = '\0';

	printf("%s\n", buf);

	return 0;
}
