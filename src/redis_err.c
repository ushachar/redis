/*
 * strerror is not thread safe
 * why oh why should I complain
 * when strerror_r & _l behave differently
 * Across the Unix domain
 *
 * after distinguishing between the two
 * And allocating error strings all over
 * You have ugly Marcos
 * (and are still not async signal safe)
 *
 * Allocate a static array containing all possible error strings
 */

#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "redis_err.h"
#include "zmalloc.h"

const char **ERROR_ARRAY;
/* sys_nerr  is considered internal impl. detail, and isn't available on all OSes */
const unsigned int MAX_ERR = 256;
const char *UNKNOWN_STRING = "Unknown error";

/*
 * Initialize the error array
 */
int initErrorStrings(){
    ERROR_ARRAY = zmalloc(sizeof(char*)*(MAX_ERR));

    unsigned int n = 0;
    char *err;

    for (; n < MAX_ERR; n++) {
        err = strerror(n);
        if (err == NULL
            || strncasecmp(err, UNKNOWN_STRING, 13) == 0)
        {
            ERROR_ARRAY[n] = UNKNOWN_STRING;
            continue;
        }
        ERROR_ARRAY[n] = zstrdup(err);
    }
    return 0;
}


const char *redisError(unsigned int err) {
    if (err > MAX_ERR) {
        perror("Requested out of bounds error string");
        return UNKNOWN_STRING;
    }
    return ERROR_ARRAY[err];
}


void freeErrorStrings() {
    unsigned int n = 0;
    for (;n < MAX_ERR; n++) {
        if (ERROR_ARRAY[n] != UNKNOWN_STRING) {
            zfree((void *)ERROR_ARRAY[n]);
        }
    }

}