/*
 * Author: Herbert (wang_xiaoq@126.com)
 * License: GPL v2
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <string.h>
#include <libaio.h>
#include <pthread.h>

#define FATAL(s) do { \
    perror(s); \
    exit(-1); \
} while (0)

#define FILENAME  "/tmp/test"

#define NR_EVENT 4096

#define MEMALIGN 512
#define BLKSIZE 512

#define STR "abcdefghijklmnopqrstuvwxyz"

/* file descriptor to manipulate a file */
int file_des = -1;
/* eventfd file descriptor */
int efd = -1;
/* epoll file descriptor */
int epfd = -1;

io_context_t aio_ctx;
struct iocb iocbs[NR_EVENT];
struct io_event io_events[NR_EVENT];

int write_done_callback(struct iocb *iocbp, long res, long res2)
{
    printf("iocb data: %ld, res: %ld, res2: %ld\n", (long)iocbp->data, res, res2);

    return 0;
}

void *submit_io_thread_func(void *arg)
{
    int ret = 0;
    struct iocb *iocbps[NR_EVENT];
    long i = 0;

    void *buf = NULL;
    ret = posix_memalign(&buf, MEMALIGN, BLKSIZE);
    if (0 != ret) {
        FATAL("posix_memalign");
    }

    while (1) {
        for (i = 0; i < NR_EVENT; i++) {
            iocbps[i] = &iocbs[i];
            io_prep_pwrite(iocbps[i], file_des, buf, BLKSIZE, i*BLKSIZE);
            io_set_eventfd(iocbps[i], efd);
            /* some private data */
            iocbps[i]->data = (void *)i;
        }

        ret = io_submit(aio_ctx, NR_EVENT, iocbps);
        if (NR_EVENT != ret) {
            FATAL("io_sbuimt");
        }

        sleep(1);
    }

    return NULL;
}

void *epoll_io_thread_func(void *arg)
{
    uint64_t finished_io = 0;
    int i = 0;
    int ret = 0;
    struct epoll_event ep_event;
    memset(&ep_event, 0, sizeof(ep_event));

    while (1) {
        ret = epoll_wait(epfd, &ep_event, 1, -1);
        if (ret < 0) {
            FATAL("epoll_wait");
        }

        ret = read(efd, &finished_io, sizeof(finished_io));
        if (sizeof(finished_io) != ret) {
            FATAL("read");
        }

        while (finished_io > 0) {
            struct timespec tms = {0, 0};
            ret = io_getevents(aio_ctx, 1, finished_io, io_events, &tms);
            if (ret > 0) {
                for (i = 0; i < ret; i++) {
                    write_done_callback(io_events[i].obj, io_events[i].res, io_events[i].res2);
                }
                finished_io -= ret;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int ret = 0;
    pthread_t t1, t2;
    struct epoll_event ep_event;
    memset(&ep_event, 0, sizeof(ep_event));

    file_des = open(FILENAME, O_RDWR | O_DIRECT | O_CREAT, 0644);
    if (file_des < 0) {
        FATAL("open");
    }

    efd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (efd < 0) {
        FATAL("eventfd");
    }

    epfd = epoll_create(1);
    if (epfd < 0) {
        FATAL("epoll_create");
    }
    ep_event.events = EPOLLIN | EPOLLET;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, efd, &ep_event);
    if (ret < 0) {
        FATAL("epoll_ctl");
    }

    memset(&aio_ctx, 0, sizeof(aio_ctx));
    ret = io_setup(NR_EVENT, &aio_ctx);
    if (ret < 0) {
        FATAL("io_setup");
    }

    pthread_create(&t1, NULL, submit_io_thread_func, NULL);
    pthread_create(&t2, NULL, epoll_io_thread_func, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
