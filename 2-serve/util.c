#include "util.h"

void read_file(char *filename, char *dest) {
    char *buffer;
    FILE *file = fopen(filename, "r");
    if (file) {
        fseek(file, 0, SEEK_END);
        long length = ftell(file);
        fseek(file, 0, SEEK_SET);

        buffer = malloc(length);
        if (buffer) {
            fread(buffer, 1, length, file);
        }
        fclose(file);
    }
    strcat(dest, buffer);

    free(buffer);
}
