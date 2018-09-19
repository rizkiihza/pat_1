#include "router.h"

void route(char *request, char *response) {
    char path[32];
    int parse_result = parse_path(request, path);
    if (parse_result == -1) {
        strcpy(response, "HTTP/1.1 405 Method Not Allowed");
        return;
    }

    if (strcmp(path, "/") == 0) {
        strcpy(response, "HTTP/1.1 200 OK");
        read_file("templates/index.html", response);
    } else if (strcmp(path, "/big") == 0) {
        strcpy(response, "HTTP/1.1 200 OK");
        read_file("templates/big.html", response);
    } else if (strcmp(path, "/small") == 0) {
        strcpy(response, "HTTP/1.1 200 OK");
        read_file("templates/small.html", response);
    } else {
        strcpy(response, "HTTP/1.1 404 Not Found");
    }
}

int parse_path(char *request, char *path) {
    char *start = strstr(request, "GET");
    if (start == NULL) {
        return -1;
    }
    start += strlen("GET ");

    char *end = start;
    for (; *end != ' '; end++)
        ;

    strncpy(path, start, end - start);
    path[end - start] = '\0';

    return 0;
}
