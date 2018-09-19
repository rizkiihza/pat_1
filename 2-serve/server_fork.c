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
#include <fcntl.h>

// utilities
#include "util.h"

#define PORT 8080
#define HOST "127.0.0.1"

#define MAX_CONNECTION 1000

#define BUFFER_SIZE 2048

int main(int argc, char *argv[])
{
    // initialize socket
    int server_fd = socket(/* IPv4 */ AF_INET, /* TCP */ SOCK_STREAM, /* IP */ 0);
    if (server_fd < 0)
    {
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
    int bind_result = bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    if (bind_result == -1)
    {
        perror("Sayang sekali, binding socket fd dengan address gagal");
        close(server_fd);
        exit(1);
    }

    // listen
    int listen_result = listen(server_fd, MAX_CONNECTION);
    if (listen_result == -1)
    {
        perror("Sayang sekali, server gagal mendengarkan");
        close(server_fd);
        exit(1);
    }
    printf("Wah, server mendengarkan permintaan di port: %d\n", port);

    // fork loop
    while (1)
    {
        // accept connection
        struct sockaddr_in client_address;
        socklen_t client_address_length = sizeof(client_address);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_length);
        if (client_fd == -1)
        {
            perror("Sayang sekali, gagal menerima permintaan klien");
            continue;
        }

        // handle every client in different processes
        if (fork() == 0)
        {
            // read request
            close(server_fd);
            char request[BUFFER_SIZE];
            memset(request, 0, sizeof(request));
            read(client_fd, request, sizeof(request));

            printf("%s\n", request);

            // handle request
            char response[BUFFER_SIZE];
            char filename[] = "index.html";
            strcpy(response, "HTTP/1.1 OK 200");

            read_file(filename, response);
            write(client_fd, response, strlen(response));

            // close connection
            close(client_fd);
            exit(0);
        }

        // parent
        close(client_fd);
    }

    return 0;
}
