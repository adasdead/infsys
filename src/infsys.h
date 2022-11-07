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

#endif /* _WIN32 */

#if _WIN32
#define INFSYS_MAIN_RUN(func)                           \
    int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev,  \
                       PSTR line, int show)             \
    {                                                   \
        return func;                                    \
    }
#else /* _WIN32 */
#define INFSYS_MAIN_RUN(func)                           \
    int main(void)                                      \
    {                                                   \
        return func;                                    \
    }

#endif

#define VERSION       "0.1"

#define WINDOW_NAME   "infsys"
#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 500

#define SIZE_KB       1024

typedef char *string;

#define s_strlen(str) ((str) ? strlen(str) : 0)

#define str_trimr(str)                                  \
    do {                                                \
        if (!str) break;                                \
        string tmp = str + s_strlen(str);               \
        while (isspace(*--tmp));                        \
        tmp[1] = '\0';                                  \
    } while (0);

#endif /* INFSYS_H */
