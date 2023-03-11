/*
    This file should never be used by someone
    using the library, it's internal and used
    for IO operations (mostly for files).
*/

#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>

extern char _fell__buffer[4208];

FILE * _fell__open(const char *, const char *);
size_t _fell__close(FILE *);
double _fell__read_number(FILE *);
char * _fell__read_line(FILE *);
void   _fell__read_file(FILE *, char **);
void   _fell__free(char **);

#endif
