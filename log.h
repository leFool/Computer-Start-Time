#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <ERRNO.H>
#include "date.h"

#define DATE_SIZE (27)
#define PATH_SLOG ("C:\\Cst")
#define DIRECTORY_SLOG ("C:\\Cst\\log.txt")
#define DIRECTORY_SLAST ("C:\\Cst\\last.txt")

int setlog(int);
void removelog();
void closelog(int);
void savedate();
char *getlast();
char *readlast();
void printlog();

enum {
    log, last, loglegnth
};

typedef FILE **Log;

#endif // LOG_H_INCLUDED
