#ifndef _CPU_CPUCACHE_H
#define _CPU_CPUCACHE_H

#include "cpu/cpuid.h"
#include "cpu/cpuinfo.h"

typedef enum
{
    CACHE_L1D, CACHE_L1I,
    CACHE_L2, CACHE_L3

} cache_type_t;

typedef struct
{
    cache_type_t type;

    int32_t line_size;
    int32_t phys_line;
    int32_t ways;
    int32_t sets;

} cache_t;

typedef cache_t cacheset_t[4];

static void cpucache_update(cacheset_t set)
{
    registers32_t regs = {0};

    for (int i = 0; i < 4; i++)
    {
        if (!cpuinfo_is_intel())
            cpuid(0x8000001D, i, &regs);
        else
            cpuid(0x4, i, &regs);

        printf("%d\n", (regs.eax >> 14) & 0xF);
        
        int32_t cache_type = regs.eax & 0xF;
        int32_t cache_lvl = (regs.eax >> 5) & 0x7;
        
        int32_t cache_line_size = (regs.ebx & 0xFF) + 1;
        int32_t cache_phys_line = ((regs.ebx >> 12) & 0xFF) + 1;
        int32_t cache_ways = (regs.ebx >> 22) + 1;
        int32_t cache_sets = regs.ecx + 1;

        size_t index = (cache_lvl + (cache_type >= 2)) - 1;

        set[index] = (cache_t) {
            (cache_type_t) index, cache_line_size,
            cache_phys_line, cache_ways, cache_sets
        };
    }
}

#endif // _CPU_CPUCACHE_H