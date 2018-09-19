#include "util.h"

#define CHUNK_SIZE 1024

void read_file(char *filename, char *buffer)
{
    char chunk[CHUNK_SIZE];
    FILE *file = fopen(filename, "r");
    if (file)
    {
        while (fread(chunk, 1, sizeof chunk, file) > 0) {
            strcat(buffer, chunk);
        }
    }
    fclose(file);
}
