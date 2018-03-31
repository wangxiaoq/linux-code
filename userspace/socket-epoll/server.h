#ifndef SERVER_H
#define SERVER_H

#define CLIENTMAX 128

#define CLIENT_UNCONN (-1)

/**
 * type of client
 */
typedef struct _client {
    int client_sock_fd;
} client_t;

#endif
