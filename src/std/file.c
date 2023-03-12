#include "file.h"

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

int _fell__read_number(FILE * file, double * value) {
    if(fscanf_s(file, "%lf", value) > 0)
        return 1;
    else
        return 0;
}

int _fell__read_line(FILE * file, char * buffer) {
    const char * new = fgets(buffer, 4208, file);

    if(new == NULL)
        return 0;
    else
        return 1;
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
