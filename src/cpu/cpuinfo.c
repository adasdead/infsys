#include "cpu/cpuinfo.h"
#include "cpu/cpucache.h"
#include "cpu/cpuid.h"

#define TEMP_START_SIZE                             250

#define BIT_TEST(num, n) (((num) >> (n)) & 1)
#define BITS_TO_STR(num) (cstring_t) (&num)

void cpuinfo_identify()
{
    registers32_t regs = {0};

    memset(&cpuinfo, 0, sizeof(cpuinfo));
    
    cpuid(0x0, 0, &regs);

    memcpy(cpuinfo.vendor + 0, BITS_TO_STR(regs.ebx), 4);
    memcpy(cpuinfo.vendor + 4, BITS_TO_STR(regs.edx), 4);
    memcpy(cpuinfo.vendor + 8, BITS_TO_STR(regs.ecx), 4);

    cpuid(0x1, 0, &regs);

    union {
        uint32_t value;

        struct
        {
            uint32_t stepping   : 4;
            uint32_t model      : 4;
            uint32_t family     : 4;
            uint32_t ext_model  : 4;
            uint32_t ext_family : 8;
        };

    } signature = { .value = regs.eax };

    cpuinfo.stepping = signature.stepping;
    cpuinfo.model = signature.model;
    cpuinfo.family = signature.family;
    cpuinfo.ext_model = signature.ext_model;
    cpuinfo.ext_family = signature.ext_family;

    cpuinfo.features[F_SSE3] = BIT_TEST(regs.ecx, 0);
    cpuinfo.features[F_PCLMULQDQ] = BIT_TEST(regs.ecx, 1);
    cpuinfo.features[F_MONITOR] = BIT_TEST(regs.ecx, 3);
    cpuinfo.features[F_SSSE3] = BIT_TEST(regs.ecx, 9);
    cpuinfo.features[F_FMA] = BIT_TEST(regs.ecx, 12);
    cpuinfo.features[F_CMPXCHG16B] = BIT_TEST(regs.ecx, 13);
    cpuinfo.features[F_SSE4_1] = BIT_TEST(regs.ecx, 19);
    cpuinfo.features[F_SSE4_2] = BIT_TEST(regs.ecx, 20);
    cpuinfo.features[F_MOVBE] = BIT_TEST(regs.ecx, 22);
    cpuinfo.features[F_POPCNT] = BIT_TEST(regs.ecx, 23);
    cpuinfo.features[F_AES] = BIT_TEST(regs.ecx, 25);
    cpuinfo.features[F_XSAVE] = BIT_TEST(regs.ecx, 26);
    cpuinfo.features[F_OSXSAVE] = BIT_TEST(regs.ecx, 27);
    cpuinfo.features[F_AVX] = BIT_TEST(regs.ecx, 28);
    cpuinfo.features[F_F16C] = BIT_TEST(regs.ecx, 29);
    cpuinfo.features[F_RDRAND] = BIT_TEST(regs.ecx, 30);

    cpuinfo.features[F_MSR] = BIT_TEST(regs.edx, 5);
    cpuinfo.features[F_CX8] = BIT_TEST(regs.edx, 8);
    cpuinfo.features[F_SEP] = BIT_TEST(regs.edx, 11);
    cpuinfo.features[F_CMOV] = BIT_TEST(regs.edx, 15);
    cpuinfo.features[F_CLFSH] = BIT_TEST(regs.edx, 19);
    cpuinfo.features[F_MMX] = BIT_TEST(regs.edx, 23);
    cpuinfo.features[F_FXSR] = BIT_TEST(regs.edx, 24);
    cpuinfo.features[F_SSE] = BIT_TEST(regs.edx, 25);
    cpuinfo.features[F_SSE2] = BIT_TEST(regs.edx, 26);
    cpuinfo.features[F_HTT] = BIT_TEST(regs.edx, 28);

    cpuinfo.features[F_PREFETCHWT1] = BIT_TEST(regs.ecx, 0);

    cpuinfo.threads = (regs.ebx >> 16) & 0xFF;

    if (IS_INTEL())
    {
        cpuid(0x4, 0, &regs);

        cpuinfo.cores = ((regs.eax >> 26) & 0x3F) + 1;
    }
    else
    {
        cpuid(0x80000008, 0, &regs);

        cpuinfo.cores = (regs.ecx & 0xFF) + 1;

        if (cpuinfo.features[F_HTT])
            cpuinfo.cores /= 2;
    }

    cpuid(0x7, 0, &regs);

    cpuinfo.features[F_FSGSBASE] = BIT_TEST(regs.ebx, 0);
    cpuinfo.features[F_BMI1] = BIT_TEST(regs.ebx, 3);
    cpuinfo.features[F_HLE] = IS_INTEL() && BIT_TEST(regs.ebx, 4);
    cpuinfo.features[F_AVX2] = BIT_TEST(regs.ebx, 5);
    cpuinfo.features[F_BMI2] = BIT_TEST(regs.ebx, 8);
    cpuinfo.features[F_ERMS] = BIT_TEST(regs.ebx, 9);
    cpuinfo.features[F_INVPCID] = BIT_TEST(regs.ebx, 10);
    cpuinfo.features[F_RTM] = IS_INTEL() && BIT_TEST(regs.ebx, 11);
    cpuinfo.features[F_RDSEED] = BIT_TEST(regs.ebx, 18);
    cpuinfo.features[F_ADX] = BIT_TEST(regs.ebx, 19);
    cpuinfo.features[F_SHA] = BIT_TEST(regs.ebx, 29);

    cpuid(0x80000001, 0, &regs);

    cpuinfo.features[F_LAHF] = BIT_TEST(regs.ecx, 0);
    cpuinfo.features[F_LZCNT] = IS_INTEL() && BIT_TEST(regs.ecx, 5);
    cpuinfo.features[F_ABM] = !IS_INTEL() && BIT_TEST(regs.ecx, 5);
    cpuinfo.features[F_SSE4a] = !IS_INTEL() && BIT_TEST(regs.ecx, 6);
    cpuinfo.features[F_XOP] = !IS_INTEL() && BIT_TEST(regs.ecx, 11);
    cpuinfo.features[F_TBM] = !IS_INTEL() && BIT_TEST(regs.ecx, 21);

    cpuinfo.features[F_TBM] = IS_INTEL() && BIT_TEST(regs.edx, 11);
    cpuinfo.features[F_MMXEXT] = !IS_INTEL() && BIT_TEST(regs.edx, 22);
    cpuinfo.features[F_RDTSCP] = IS_INTEL() && BIT_TEST(regs.edx, 27);
    cpuinfo.features[F_3DNOWEXT] = !IS_INTEL() && BIT_TEST(regs.edx, 30);
    cpuinfo.features[F_3DNOW] = !IS_INTEL() && BIT_TEST(regs.edx, 31);

    size_t offset = 0;

    for (int i = 0x80000002; i <= 0x80000004; i++)
    {
        cpuid(i, 0, &regs);

        memcpy(cpuinfo.name + offset + 0,  BITS_TO_STR(regs.eax), 4);
        memcpy(cpuinfo.name + offset + 4,  BITS_TO_STR(regs.ebx), 4);
        memcpy(cpuinfo.name + offset + 8,  BITS_TO_STR(regs.ecx), 4);
        memcpy(cpuinfo.name + offset + 12, BITS_TO_STR(regs.edx), 4);

        offset += 16;
    }

    cpucache_update(cpuinfo.caches);
}

void cpuinfo_features_str(string_t dest, size_t size)
{
    static cstring_t feature_names[F_TOTAL] = {
        "3DNow!", "3DNow!(+)", "ABM", "ADX", "AES", "AVX", "AVX2",
        "BMI1", "BMI2", "CLFSH", "CMPXCHG16B", "CX8", "CMOV", "ERMS",
        "F16C", "FMA", "FSGSBASE", "FXSR", "HLE", "HTT", "INVPCID",
        "LAHF", "LZCNT", "MMX", "MMX(+)", "MONITOR", "MOVBE", "MSR",
        "OSXSAVE", "PCLMULQDQ", "POPCNT", "PREFETCHWT1", "RDRAND", "RDSEED",
        "RDTSCP", "RTM", "SEP", "SHA", "SSE", "SSE2", "SSE3", "SSE4.1",
        "SSE4.2", "SSE4a", "SSSE3", "SYSCALL", "TBM", "XOP", "XSAVE"
    };

    string_t temp = malloc(TEMP_START_SIZE);
    size_t temp_len = 0;

    memset(temp, 0, TEMP_START_SIZE);

    for (int i = 0; i < F_TOTAL; i++)
    {
        if (cpuinfo.features[i])
        {
            switch (i)
            {
            case F_MMX:
                if (cpuinfo.features[F_MMXEXT])
                    continue;
                break;
            
            case F_3DNOW:
                if (cpuinfo.features[F_3DNOWEXT])
                    continue;
                break;
            }

            cstring_t feature = feature_names[i];
            size_t feature_len = strlen(feature);
            size_t new_size = temp_len + feature_len + 2;

            if (new_size > TEMP_START_SIZE)
            {
                string_t result = realloc(temp, new_size);

                if (result) temp = result;
                else goto err;
            }
            
            strncpy(temp + temp_len, feature, feature_len);
            temp_len += feature_len;

            strncpy(temp + temp_len, ", ", 2);
            temp_len += 2;
        }
    }

err:
    temp[temp_len - 2] = '\0';

    strncpy(dest, temp, size);
    free(temp);
}