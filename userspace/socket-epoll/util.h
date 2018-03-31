#ifndef UTIL_H
#define UTIL_H

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#define SERVADDR "127.0.0.1"
#define SERVPORT 6666

#define BUFFSIZE 4096

#define FATAL(s) do { \
    fprintf(stderr, "%s: %s\n", s, strerror(errno)); \
    exit(-1); \
} while (0)

#endif
