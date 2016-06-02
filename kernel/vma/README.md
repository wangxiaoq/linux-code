# vma-test
vma-test is a kernel module implemented the mmap system call, and can map a page to user space. Thus applications can manipulate the page allocated in kernel module.

## Usage:

```
1. make
2. make install
3. gcc userspace.c
4. cat /dev/vma-test
5. sudo ./a.out
6. cat /dev/vma-test
```
