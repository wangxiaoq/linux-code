#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>

#include "util.h"

/*
 * return value: seccess - the socket fd, fail - exit program;
 */
static int client_init(int domain, int type, struct sockaddr *saddr, socklen_t len)
{
    int ret = 0;
    int sock_fd = socket(domain, type, 0);
    if (sock_fd < 0) {
        FATAL("client socket error");
    }

    ret = connect(sock_fd, saddr, len);
    if (ret < 0) {
        FATAL("client connect error");
    }

    return sock_fd;
}

int main(void)
{
    int sock_fd = -1;
    char buf[BUFFSIZE] = {'\0'};
    int recv_size = 0;
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(SERVPORT),
        .sin_addr = inet_addr(SERVADDR),
    };

    sock_fd = client_init(AF_INET, SOCK_STREAM, (struct sockaddr*)&addr, sizeof(addr));

    while ((recv_size = read(sock_fd, buf, BUFFSIZE)) > 0) {
        recv_size = write(STDOUT_FILENO, buf, recv_size);
        if (recv_size < 0) {
            FATAL("client write error");
        }

        memset(buf, 0, BUFFSIZE);
    }

    return 0;
}
