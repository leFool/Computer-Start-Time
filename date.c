#include "date.h"

char *getdate() {
    time_t t = time(NULL);
    return ctime(&t);
}
