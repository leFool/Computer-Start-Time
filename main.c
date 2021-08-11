#include <stdio.h>
#include <string.h>
#include "registry.h"
#include "log.h"

int streq(const char *, const char *);

int main(int varc, char **argv) {
    if(varc <= 1)
        return 0;
    if(streq(argv[1], "-h")) {
        printlog();
        return 0;
    }
    if(streq(argv[1], "-start")) {
        printf(installtoregistry() ?
               ("the tracker has started\n") : ("failed to start\n"));
        return 0;
    }
    if(streq(argv[1], "-stop")) {
        printf(uninstallfromregistry() ?
               ("the tracker has stopped\n") : ("failed to stop\n"));
        return 0;
    }
    if(streq(argv[1], "-c")) {
        printf("Are you sure you want to clear the log? (Y/N) ");
        char c = getchar();
        if(c != 'Y' && c != 'y')
            return 0;
        removelog();
        printf("history cleared\n");
        return 0;
    }
    int islast = streq(argv[1], "-l");
    if (!setlog(islast)) {
        printf("Couldn't create or open logs, exiting\n");
        return 0;
    }
    if(islast) {
        char *s = readlast();
        if(s) {
            printf("Last time this computer was started: %s\n", s);
            free(s);
        } else
            printf("No was data found\n");
    } else
        if(streq(argv[1], "-r"))
            savedate();
    closelog(islast);
    return 0;
}

int streq(const char *s1, const char *s2) {
    return (strcmp(s1, s2) == 0);
}
