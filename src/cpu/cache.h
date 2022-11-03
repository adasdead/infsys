#ifndef CPU_CACHE_H
#define CPU_CACHE_H

#include "infsys.h"

enum cache_type
{
    CACHE_L1D, CACHE_L1I, CACHE_L2, CACHE_L3
};

struct cpu_cache
{
    enum cache_type type;

    int32_t size;

    int32_t line_size;
    int32_t phys_line;
    int32_t ways;
    int32_t sets;
};

void cpu_cache_update(struct cpu_cache caches[4], bool intel);

#endif /* CPU_CACHE_H */
