#ifndef _CPU_CPUCACHE_H
#define _CPU_CPUCACHE_H

#include "infsys.h"

typedef enum
{
    CACHE_L1D, CACHE_L1I,
    CACHE_L2, CACHE_L3

} cache_type_t;

typedef struct
{
    cache_type_t type;

    int32_t size;

    int32_t line_size;
    int32_t phys_line;
    int32_t ways;
    int32_t sets;

} cache_t;

typedef cache_t cacheset_t[4];

void cpucache_update(cacheset_t set);

#endif // _CPU_CPUCACHE_H