#ifndef INFSYS_H
#define INFSYS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#if _WIN32

#ifndef UNICODE
#define UNICODE
#endif /* UNICODE */

#include <windows.h>

#define INFSYS_MAIN_RUN(func)                           \
    int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev,  \
                       PSTR line, int show)             \
    {                                                   \
        return func;                                    \
    }
#else
#define INFSYS_MAIN_RUN(func)                           \
    int main(void)                                      \
    {                                                   \
        return func;                                    \
    }

#endif /* _WIN32 */

#define VERSION       "0.1"

#define WINDOW_NAME   "infsys"
#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 500

#define SIZE_KB       1024

typedef char *string;

#define str_trimr(str)                                  \
    do {                                                \
        string tmp = str + strlen(str);                 \
        while (isspace(*--tmp));                        \
        tmp[1] = '\0';                                  \
    } while (0);

#endif /* INFSYS_H */
