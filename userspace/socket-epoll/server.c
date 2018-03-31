#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#include "util.h"
#include "server.h"


/* number of clients connected */
static int connects_number = 0;

static pthread_mutex_t connect_clients_mutex;

static client_t connect_clients[CLIENTMAX];

static int accept_sock_fd = -1;

static inline void connect_clients_mutex_init(void)
{
    pthread_mutex_init(&connect_clients_mutex, NULL);
}

static inline void connect_clients_init(void)
{
    int i = 0;
    for (i = 0; i < CLIENTMAX; i++) {
        connect_clients[i].client_sock_fd = CLIENT_UNCONN;
    }

    connect_clients_mutex_init();
}

/* this function needs the connect_clients_mutex lock */
static int add_clients(int client_fd)
{
    int i = 0;

    if (connects_number >= CLIENTMAX)
        return -1;

    for (i = 0; i < CLIENTMAX; i++) {
        if (connect_clients[i].client_sock_fd == CLIENT_UNCONN) {
            connect_clients[i].client_sock_fd = client_fd;
            break;
        }
    }

    connects_number++;

    return 0;
}


/**
 * return value: success - socket fd, fail - exit program
 */
static int server_init(int domain, int type, struct sockaddr *saddr, socklen_t len, int backlog)
{
    int ret = 0;
    int on = 1;

    connect_clients_init();

    int sock_fd = socket(domain, type, 0);
    if (sock_fd < 0) {
        FATAL("server socket error");
    }

    ret = bind(sock_fd, saddr, len);
    if (ret < 0) {
        FATAL("server bind error");
    }

    ret = listen(sock_fd, backlog);
    if (ret < 0) {
        FATAL("server listen error");
    }

    ret = setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));
    if (ret < 0) {
        FATAL("server setsockopt error");
    }

    return sock_fd;
}

/**
 * 1st thread needs to send messages to all clients connected periodcally.
 */
void *send_messages_to_clients_thread(void *arg)
{
    int i = 0;
    int ret = 0;
    char buf[BUFFSIZE] = {'\0'};

    while (1) {
        pthread_mutex_lock(&connect_clients_mutex);
        for (i = 0; i < CLIENTMAX; i++) {
            if (connect_clients[i].client_sock_fd != CLIENT_UNCONN) {
                sprintf(buf, "hello: %d\n", connect_clients[i].client_sock_fd);
                ret = write(connect_clients[i].client_sock_fd, buf, BUFFSIZE);
                if (ret < 0) {
                    if (errno == EPIPE) {
                        connect_clients[i].client_sock_fd = CLIENT_UNCONN;
                        continue;
                    }
                    pthread_mutex_unlock(&connect_clients_mutex);
                    FATAL("server thread 1st write error");
                }
            }
        }
        pthread_mutex_unlock(&connect_clients_mutex);

        sleep(2);
    }

    return NULL;
}


/**
 * 2nd thread needs to accept the connect requests from clients.
 */
void *accept_clients_connect_thread(void *arg)
{
    /* epoll init */
    int ret = 0;
    struct epoll_event ep_event, ep_event_wait;
    int client_fd = -1;
    int i = 0;
    int epfd = epoll_create(1);
    if (epfd < 0) {
        FATAL("server thread 2nd epoll_create error");
    }

    memset(&ep_event, 0, sizeof(struct epoll_event));
    ep_event.events = EPOLLIN | EPOLLET;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, accept_sock_fd, &ep_event);
    if (ret < 0) {
        FATAL("server thread 2nd epoll_ctl error");
    }

    while (1) {
        /* epoll wait */
        memset(&ep_event_wait, 0, sizeof(struct epoll_event));
        ret = epoll_wait(epfd, &ep_event_wait, 1, -1);
        if (ret < 0) {
            FATAL("server 2nd thread epoll_wait error");
        }

        client_fd = accept(accept_sock_fd, NULL, NULL);
        if (client_fd < 0) {
            FATAL("server 2nd thread accept error");
        }

        pthread_mutex_lock(&connect_clients_mutex);
        ret = add_clients(client_fd);
        if (ret < 0) {
            pthread_mutex_unlock(&connect_clients_mutex);
            FATAL("server 2nd thread add clients error");
        }
        pthread_mutex_unlock(&connect_clients_mutex);
    }

    return NULL;
}

int main(void)
{
    pthread_t tid1, tid2;
    int ret = 0;
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(SERVPORT),
        .sin_addr = inet_addr(SERVADDR),
    };

    accept_sock_fd = server_init(AF_INET, SOCK_STREAM, (struct sockaddr *)&addr, sizeof(addr), CLIENTMAX);

    ret = pthread_create(&tid1, NULL, send_messages_to_clients_thread, NULL);
    if (ret != 0) {
         FATAL("server pthread_create 1st thread error");
    }
    ret = pthread_create(&tid2, NULL, accept_clients_connect_thread, NULL);
    if (ret != 0) {
        FATAL("server pthread_create 2nd thread error");
    }

    ret = pthread_join(tid1, NULL);
    if (ret != 0) {
        FATAL("server pthread_join 1st thread error");
    }
    ret = pthread_join(tid2, NULL);
    if (ret != 0) {
        FATAL("server pthread_join 2nd thread error");
    }

    return 0;
}
