#ifndef INFSYS_H
#define INFSYS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define VERSION       "0.1"

#define WINDOW_NAME   "infsys"
#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 500

#define SIZE_KB       1024

typedef char *string;

#define str_trimr(str)                  \
    do {                                \
        string tmp = str + strlen(str); \
        while(isspace(*--tmp));         \
        tmp[1] = '\0';                  \
    } while (0);

#endif /* INFSYS_H */
