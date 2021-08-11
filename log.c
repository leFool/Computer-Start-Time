#include "log.h"

Log sdlog;

int setlog(int islast) {
    sdlog = malloc(sizeof(FILE *) * loglegnth);
    if (!sdlog)
        return 0;
    if(islast)
        return 1;
    _set_errno(0);
    if (_mkdir(PATH_SLOG) < 0) {
        if (errno != EEXIST) {
            free(sdlog);
            return 0;
        }
    }
    sdlog[log] = fopen(DIRECTORY_SLOG, "a+");
    if (!sdlog[log]) {
        free(sdlog);
        return 0;
    }
    fseek(sdlog[log], 0, SEEK_SET);
    sdlog[last] = fopen(DIRECTORY_SLAST, "w");
    if (!sdlog[last]) {
        fclose(sdlog[log]);
        free(sdlog);
        return 0;
    }
    return 1;
}

void removelog() {
    remove(DIRECTORY_SLOG);
    remove(DIRECTORY_SLAST);
}

void closelog(int islast) {
    if(!islast) {
        int i;
        for(i = 0; i < loglegnth; ++i)
            fclose(sdlog[i]);
    }
    free(sdlog);
}

void savedate() {
    char *l = getlast();
    if(l != NULL) {
        fprintf(sdlog[last], "%s", l);
        free(l);
    }
    fprintf(sdlog[log], "%s", getdate());
}

char *getlast() {
    int flag = 0;
    char *buf;
    buf = malloc(sizeof(char) * DATE_SIZE);
    while(fgets(buf, DATE_SIZE, sdlog[log]) != NULL)
        if(!flag)
            flag = 1;
    if(flag)
        return buf;
    free(buf);
    return NULL;
}

char *readlast() {
    sdlog[last] = fopen(DIRECTORY_SLAST, "r");
    char *buf;
    buf = malloc(sizeof(char) * DATE_SIZE);
    if (!buff)
        return 0;
    if(fgets(buf, DATE_SIZE, sdlog[last]) != NULL) {
        fclose(sdlog[last]);
        return buf;
    }
    sdlog[log] = fopen(DIRECTORY_SLOG, "r");
    int flag = 0;
    while(fgets(buf, DATE_SIZE, sdlog[log]) != NULL)
        if(!flag)
            flag = 1;
    int i;
    for(i = 0; i < loglegnth; ++i) {
        fclose(sdlog[i]);
    }
    if(flag)
        return buf;
    free(buf);
    return NULL;
}

void printlog() {
    FILE *l = fopen(DIRECTORY_SLOG, "r");
    char *d = malloc(sizeof(char) * DATE_SIZE);
    while(fgets(d, DATE_SIZE, l) != NULL)
        printf("%s", d);
    printf("END OF HISTORY\n");
    free(d);
    fclose(l);
}
