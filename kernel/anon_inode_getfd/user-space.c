#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdlib.h>

#define FILENAME "/dev/anon-file"
#define MAGIC 'A'
#define CREAT_ANON_FILE _IOW(MAGIC, 0, unsigned long)

#define FATAL(s) do { \
	fprintf(stderr, "%s\n", s); \
	exit(-1); \
} while(0)

int main(void)
{
	int ret = 0;
	char buf[200] = {'\0'};
	int fd = open(FILENAME, O_RDWR);
	
	if (fd < 0)
		FATAL("open file error");

	ret = ioctl(fd, CREAT_ANON_FILE, 0);
	if (ret == -1)
		FATAL("ioctl error");
	if (read(ret, buf, 100) < 0)
		FATAL("read error");

	printf("%s\n", buf);

	return 0;
}
