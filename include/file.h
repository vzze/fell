/*
    This file should never be used by someone
    using the library, it's internal and used
    for IO operations (mostly for files).
*/

#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE * _fell__open(const char *, const char *);
size_t _fell__close(FILE *);
int    _fell__read_number(FILE *, double *);
int    _fell__read_line(FILE *, char *);
void   _fell__read_file(FILE *, char **);
void   _fell__free(char **);

#endif
