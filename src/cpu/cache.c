#include "cpu/cache.h"

#include "cpu/cpuid.h"
#include "cpu/info.h"

void cpu_cache_update(struct cpu_cache caches[4], bool intel)
{
    struct registers32 regs = {0};

    for (int i = 0; i < 4; i++)
    {
        if (!intel)
            cpuid(0x8000001D, i, &regs);
        else
            cpuid(0x4, i, &regs);

        int32_t cache_type = regs.eax & 0xF;
        int32_t cache_lvl = (regs.eax >> 5) & 0x7;

        int32_t cache_line_size = (regs.ebx & 0xFF) + 1;
        int32_t cache_phys_line = ((regs.ebx >> 12) & 0xFF) + 1;
        int32_t cache_ways = (regs.ebx >> 22) + 1;
        int32_t cache_sets = regs.ecx + 1;

        size_t index = (cache_lvl + (cache_type >= 2)) - 1;

        int32_t cache_size = cache_line_size * cache_phys_line;
        cache_size *= cache_sets * cache_ways;

        caches[index] = (struct cpu_cache) {
            (enum cache_type) index, cache_size, cache_line_size,
            cache_phys_line, cache_ways, cache_sets
        };
    }
}
