#include "file.h"

char _fell__buffer[4208];

FILE * _fell__open(const char * file, const char * mode) {
    FILE * handle;
    if(fopen_s(&handle, file, mode) == 0)
        return handle;
    else
        return NULL;
}

size_t _fell__close(FILE * file) {
    if(fclose(file) == 0)
        return 0;
    else
        return 1;
}

double _fell__read_number(FILE * file) {
    double read = 0;
    fscanf_s(file, "%lf", &read);
    return read;
}

char * _fell__read_line(FILE * file) {
    fgets(_fell__buffer, 4208, file);
    return _fell__buffer;
}

void _fell__read_file(FILE * file, char ** string) {
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);

    fseek(file, 0, SEEK_SET);
    *string = calloc((size_t)(fsize + 1), 1);

    fread((*string), (size_t)fsize, 1, file);
}

void _fell__free(char ** ptr) {
    free(*ptr);
}
