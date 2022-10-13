#ifndef _CPU_CPUINFO_H
#define _CPU_CPUINFO_H

#include "infsys.h"

#include "cpu/cpucache.h"

#define CPU_NAME_LEN            64 + 1
#define CPU_VENDOR_LEN          12 + 1

#define IS_INTEL() \
    (strstr(cpuinfo.vendor, "Intel") != NULL)

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

struct
{
    char name[CPU_NAME_LEN], vendor[CPU_VENDOR_LEN];

    cacheset_t caches;

    size_t cores, threads;
    size_t stepping, family, model;
    size_t ext_family, ext_model;

    bool_t features[F_TOTAL];

} cpuinfo;

void cpuinfo_identify();

void cpuinfo_features_str(string_t dest, size_t size);

#endif // _CPU_CPUINFO_H