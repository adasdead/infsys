#ifndef _WINDOWS_WINAPI_H
#define _WINDOWS_WINAPI_H

#if _WIN32

#ifndef UNICODE
#define UNICODE
#endif // UNICODE

#include <Windows.h>

#include "infsys.h"

typedef wchar_t* wstring_t;

static HINSTANCE __w32_hinstance;

static inline void w32_ascii_to_wide(wstring_t dest,
                                     cstring_t src,
                                     size_t size)
{
    MultiByteToWideChar(
        CP_UTF8, 0, src, -1, dest, (int) size
    );
}

#endif // _WIN32

#endif // _WINDOWS_WINAPI_H