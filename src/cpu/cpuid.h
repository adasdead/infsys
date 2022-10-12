#ifndef _CPU_CPUID_H
#define _CPU_CPUID_H

#include "infsys.h"

#if _WIN32
#   include <intrin.h>
#else
#   include <cpuid.h>
#endif

#define BIT_TEST(num, n) (((num) >> (n)) & 1)
#define BITS_TO_STR(num) (const char*) (&num)

typedef struct
{
    uint32_t eax, ebx, ecx, edx;

} registers32_t;

static void cpuid(uint32_t lvl, uint32_t cnt,
                  registers32_t *regs)
{
#if _WIN32
    __cpuidex((int*) regs, lvl, cnt);
#else
    __cpuid_count(lvl, cnt, regs->eax, regs->ebx,
                            regs->ecx, regs->edx);
#endif
}

#endif // _CPU_CPUID_H