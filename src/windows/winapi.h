#ifndef _WINDOWS_WINAPI_H
#define _WINDOWS_WINAPI_H

#if _WIN32

#ifndef UNICODE
#define UNICODE
#endif // UNICODE

#include <Windows.h>

#include "infsys.h"

#define W32_CLASS_BUFFER_SZ                      256

typedef wchar_t* wstring_t;

static HINSTANCE __w32_hinstance;

static inline void w32_ascii_to_wide(wstring_t dest, cstring_t src,
                                     size_t size)
{
    if (!src || !dest || !size) return;

    MultiByteToWideChar(CP_UTF8, 0, src, -1, dest, (int) size);
}

static void w32_register_class(WNDPROC proc, cstring_t name)
{
    WNDCLASS wc = {0};

    wchar_t buffer[W32_CLASS_BUFFER_SZ];
    w32_ascii_to_wide(buffer, name, W32_CLASS_BUFFER_SZ);

    wc.lpfnWndProc   = proc;
    wc.hInstance     = __w32_hinstance;
    wc.lpszClassName = buffer;

    RegisterClass(&wc);
}

#endif // _WIN32

#endif // _WINDOWS_WINAPI_H