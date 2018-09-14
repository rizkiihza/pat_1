// general library
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// socket related library
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>

// libev
#include <ev.h>

#define true 1
#define BUFFER_SIZE 2048

void read_html_file(char *filename, char *buffer);
void accept_client(struct ev_loop *loop, struct ev_io *watcher, int revents);
void read_client(struct ev_loop *loop, struct ev_io *watcher, int revents);

int main(int argc, char *argv[])
{
    // initialized const
    int port = 8080;
    char host[] = "127.0.0.1";

    // server socket and address
    int on = 1;
    int server_fd;
    struct sockaddr_in server_address;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("sayang sekali, inisiasi socket gagal");
        exit(1);
    }
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr(host);
    server_address.sin_family = AF_INET;

    // binding fd to address
    int bind_result = bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address));
    if (bind_result == -1)
    {
        perror("sayang sekali, gagal binding fd dengan alamat server");
        close(server_fd);
        exit(1);
    }

    // listen
    int max_connection = 100;
    int listen_result = listen(server_fd, max_connection);
    if (listen_result == -1)
    {
        perror("sayang sekali, gagal mendengarkan");
        close(server_fd);
        exit(1);
    }

    printf("Wah, kita mendengarkan permintaan di port : %d\n", port);

    // libev
    struct ev_io w_accept;
    struct ev_loop *loop = EV_DEFAULT;
    ev_io_init(&w_accept, accept_client, server_fd, EV_READ);
    ev_io_start(loop, &w_accept);

    while (true)
    {
        ev_loop(loop, 0);
    }

    return 0;
}

// read html file into buffer
void read_html_file(char *filename, char *buffer)
{
    char str_row[2048];
    FILE *file;
    file = fopen(filename, "r");
    if (file)
    {
        while (fgets(str_row, 2048, (FILE *)file))
        {
            strcat(buffer, str_row);
        }
    }
    fclose(file);
}

// accept client connection
void accept_client(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
    // initialize variable
    struct sockaddr_in client_address;
    socklen_t client_address_length = sizeof(client_address);
    int client_fd;
    struct ev_io *w_read = (struct ev_io *)malloc(sizeof(struct ev_io));

    // event error checking
    if (EV_ERROR & revents)
    {
        perror("sayang sekali, event yang diberikan tidak valid");
        return;
    }

    // accept client request
    client_fd = accept(watcher->fd, (struct sockaddr *)&client_address, &client_address_length);
    if (client_fd == -1)
    {
        perror("gagal menerima permintaan klien");
        return;
    }

    printf("Wah kita berhasil berhubungan dengan klien\n");

    // start ev_io to read client request
    ev_io_init(w_read, read_client, client_fd, EV_READ);
    ev_io_start(loop, w_read);
}

// read client message
void read_client(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
    char request[BUFFER_SIZE];
    ssize_t read_result;

    // events error checking
    if (EV_ERROR & revents)
    {
        perror("sayang sekali, event yang diberikan tidak valid");
        return;
    }

    // read client request
    read_result = read(watcher->fd, request, sizeof(request));
    if (read_result == -1)
    {
        perror("gagal membaca request dari pengguna");
        return;
    }

    printf("%s\n", request);

    // give response
    char response[2048];
    strcpy(response, "HTTP/1.1 OK 200");
    read_html_file("index.html", response);
    write(watcher->fd, response, strlen(response));

    // close connection
    ev_io_stop(loop, watcher);
    close(watcher->fd);
    bzero(request, sizeof(request));
}