#ifndef HARDWARE_CPU_CPUID_H
#define HARDWARE_CPU_CPUID_H

#include "infsys.h"

#if _WIN32
#   include <intrin.h>
#else
#   include <cpuid.h>
#endif /* _WIN32 */

struct registers32
{
    uint32_t eax, ebx, ecx, edx;
};

static void cpuid(uint32_t lvl, uint32_t cnt,
                  struct registers32 *regs)
{
#if _WIN32
    __cpuidex((int*) regs, lvl, cnt);
#else
    __cpuid_count(lvl, cnt, regs->eax, regs->ebx,
                            regs->ecx, regs->edx);
#endif /* _WIN32 */
}

#endif /* HARDWARE_CPU_CPUID_H */
