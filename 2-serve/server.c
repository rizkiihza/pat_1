// standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// socket-related libraries
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/types.h>

// libev
#include <ev.h>

// internal libraries
#include "router.h"

#define PORT 8080
#define HOST "127.0.0.1"

#define MAX_CONNECTION 10000

#define REQUEST_SIZE 2048
#define RESPONSE_SIZE 32768

void accept_client(struct ev_loop *loop, struct ev_io *w, int revents);
void read_client(struct ev_loop *loop, struct ev_io *w, int revents);

int main(int argc, char *argv[]) {
    // initialize socket
    int server_fd =
        socket(/* IPv4 */ AF_INET, /* TCP */ SOCK_STREAM, /* IP */ 0);
    if (server_fd < 0) {
        perror("Sayang sekali, inisiasi socket gagal");
        exit(1);
    }

    int value = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));

    // set socket address
    int port = PORT;
    char host[] = HOST;

    struct sockaddr_in address;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(host);
    address.sin_family = AF_INET;

    // bind socket fd to address
    int bind_result =
        bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    if (bind_result == -1) {
        perror("Sayang sekali, binding socket fd dengan address gagal");
        close(server_fd);
        exit(1);
    }

    // listen
    int listen_result = listen(server_fd, MAX_CONNECTION);
    if (listen_result == -1) {
        perror("Sayang sekali, server gagal mendengarkan");
        close(server_fd);
        exit(1);
    }
    printf("Wah, server mendengarkan permintaan di port: %d\n", port);

    // libev
    struct ev_io accept_w;
    struct ev_loop *loop = EV_DEFAULT;
    ev_io_init(&accept_w, accept_client, server_fd, EV_READ);
    ev_io_start(loop, &accept_w);

    ev_run(loop, 0);

    return 0;
}

// accept client connection
void accept_client(struct ev_loop *loop, struct ev_io *w, int revents) {
    // event error checking
    if (EV_ERROR & revents) {
        perror("Sayang sekali, event tidak valid");
        return;
    }

    // accept client request
    struct sockaddr_in client_address;
    socklen_t length = sizeof(client_address);
    int client_fd = accept(w->fd, (struct sockaddr *)&client_address, &length);
    if (client_fd == -1) {
        perror("Sayang sekali, gagal menerima permintaan klien");
        return;
    }
    printf("Wah, server berhasil berhubungan dengan klien\n");

    // start ev_io to read client request
    struct ev_io *read_w = (struct ev_io *)malloc(sizeof(struct ev_io));
    ev_io_init(read_w, read_client, client_fd, EV_READ);
    ev_io_start(loop, read_w);
}

// read client message
void read_client(struct ev_loop *loop, struct ev_io *w, int revents) {
    // event error checking
    if (EV_ERROR & revents) {
        perror("Sayang sekali, event tidak valid");
        return;
    }

    // read client request
    char request[REQUEST_SIZE];
    ssize_t read_result = read(w->fd, request, sizeof(request));
    if (read_result == -1) {
        perror("Sayang sekali, gagal membaca permintaan klien");
        return;
    }

#ifdef VERBOSE
    printf("%s\n", request);
#endif

    // give response
    char response[RESPONSE_SIZE];
    route(request, response);
    write(w->fd, response, strlen(response));

#ifdef VERBOSE
    printf("%s\n", response);
#endif

    // close connection
    ev_io_stop(loop, w);
    close(w->fd);
}
