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

#define BUFFER_SIZE 2048

void read_html_file(char *filename, char *buffer)
{
    char str_row[BUFFER_SIZE];
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

int main(int argc, char *argv[])
{
    // initialized const
    int port = 8080;
    char host[] = "127.0.0.1";
    char http_response_ok[] = "HTTP/1.1 200 OK\r\n";
    int on = 1;

    // server socket and address
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

    // initialize client variable
    struct sockaddr_in client_address;
    int client_fd;

    while (1)
    {
        // accept connection
        socklen_t client_address_length = sizeof(client_address);
        client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_length);
        if (client_fd == -1)
        {
            perror("gagal menerima permintaan klien");
            continue;
        }

        // handle every client in different process
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
            strcpy(response, http_response_ok);

            read_html_file(filename, response);
            write(client_fd, response, strlen(response));

            // close connection
            printf("menutup koneksi\n");
            close(client_fd);
            exit(0);
        }

        // parent
        close(client_fd);
    }

    return 0;
}