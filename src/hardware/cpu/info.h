#ifndef HARDWARE_CPU_INFO_H
#define HARDWARE_CPU_INFO_H

#include "infsys.h"

#include "hardware/cpu/cache.h"

#define CPU_NAME_SIZE    64 + 1
#define CPU_VENDOR_SIZE  12 + 1

enum cpu_features
{
    F_3DNOW, F_3DNOWEXT, F_ABM, F_ADX, F_AES, F_AVX,
    F_AVX2, F_BMI1, F_BMI2, F_CLFSH, F_CMPXCHG16B,
    F_CX8, F_CMOV, F_ERMS, F_F16C, F_FMA, F_FSGSBASE,
    F_FXSR, F_HLE, F_HTT, F_INVPCID, F_LAHF, F_LZCNT,
    F_MMX, F_MMXEXT, F_MONITOR, F_MOVBE, F_MSR,
    F_OSXSAVE, F_PCLMULQDQ, F_POPCNT, F_PREFETCHWT1,
    F_RDRAND, F_RDSEED, F_RDTSCP, F_RTM, F_SEP, F_SHA,
    F_SSE, F_SSE2, F_SSE3, F_SSE4_1, F_SSE4_2, F_SSE4a,
    F_SSSE3, F_SYSCALL, F_TBM, F_XOP, F_XSAVE, F_TOTAL
};

struct cpu_info
{
    char name[CPU_NAME_SIZE];
    char vendor[CPU_VENDOR_SIZE];

    struct cpu_cache caches[4];

    size_t cores, threads;
    size_t stepping, family, model;
    size_t ext_family, ext_model;

    bool features[F_TOTAL];

    bool is_intel;
};

void cpu_identify(struct cpu_info *info);

void cpu_features_str(const struct cpu_info *info,
                      string dest, size_t size);

#endif /* HARDWARE_CPU_INFO_H */
