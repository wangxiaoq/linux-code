#include <event2/event.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <memory>
#include <iostream>
#include <strings.h>
#include <string.h>

#define PORT 34567
#define BUFSIZE 4096
#define BACKLOG 5

int start_tcp_server()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        return -1;
    }

    int on = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        return -1;
    }

    struct sockaddr_in saddr;
    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(fd, reinterpret_cast<struct sockaddr*>(&saddr), sizeof(saddr)) < 0) {
        return -1;
    }
    if (listen(fd, BACKLOG) < 0) {
        return -1;
    }

    return fd;
}

void on_read(evutil_socket_t fd, short events, void *arg)
{
    char buf[BUFSIZE] = {0};
    int ret = recv(fd, buf, BUFSIZE - 1, 0);
    if (ret < 0) {
        return ;
    }

    ret = send(fd, buf, strlen(buf), 0);
    if (ret < 0) {
        return ;
    }

    std::cout << "on_read end" <<std::endl;
}

void on_accept(evutil_socket_t fd, short events, void *arg)
{
    evutil_socket_t cfd;
    struct sockaddr_in saddr;
    bzero(&saddr, sizeof(saddr));
    socklen_t len = sizeof(saddr);
    cfd = accept(fd, reinterpret_cast<struct sockaddr*>(&saddr), &len);
    if (cfd < 0) {
        return ;
    }
    evutil_make_socket_nonblocking(cfd);

    auto ev_base_ptr = *reinterpret_cast<std::shared_ptr<event_base>*>(arg);
    event *ev = event_new(ev_base_ptr.get(), cfd, EV_READ|EV_PERSIST, on_read, NULL);
    event_add(ev, NULL);

    std::cout << "on_accept end" << std::endl;

    return;
}

int main(int argc, char *argv[])
{
    std::shared_ptr<event_base> ev_base_ptr(event_base_new(), event_base_free);
    int fd = start_tcp_server();
    if (fd < 0) {
        return -1;
    }
    evutil_make_socket_nonblocking(fd);

    std::shared_ptr<event> ev_ptr(event_new(ev_base_ptr.get(), fd, EV_READ|EV_PERSIST, on_accept, reinterpret_cast<void *>(&ev_base_ptr)), event_free);
    event_add(ev_ptr.get(), NULL);
    event_base_dispatch(ev_base_ptr.get());

    return 0;
}
