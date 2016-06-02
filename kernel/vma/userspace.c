#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

#define MODULE_NAME "/dev/vma-test"
#define PAGE_SIZE 4096

int main(void)
{
	int fd = open(MODULE_NAME, O_RDWR);
	char *addr = mmap(NULL, PAGE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	memset(addr, 'a', PAGE_SIZE);

	return 0;
}
