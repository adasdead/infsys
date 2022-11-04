#include "cpu/info.h"

#include "cpu/cache.h"
#include "cpu/cpuid.h"

#define BUFFER_SIZE 250

#define bit(num, n) (((num) >> (n)) & 1)
#define bit_str(num) (const string) (&num)

static const string feature_names[F_TOTAL] = {
    "3DNow!", "3DNow!(+)", "ABM", "ADX", "AES", "AVX", "AVX2",
    "BMI1", "BMI2", "CLFSH", "CMPXCHG16B", "CX8", "CMOV", "ERMS",
    "F16C", "FMA", "FSGSBASE", "FXSR", "HLE", "HTT", "INVPCID",
    "LAHF", "LZCNT", "MMX", "MMX(+)", "MONITOR", "MOVBE", "MSR",
    "OSXSAVE", "PCLMULQDQ", "POPCNT", "PREFETCHWT1", "RDRAND", "RDSEED",
    "RDTSCP", "RTM", "SEP", "SHA", "SSE", "SSE2", "SSE3", "SSE4.1",
    "SSE4.2", "SSE4a", "SSSE3", "SYSCALL", "TBM", "XOP", "XSAVE"
};

void cpu_identify(struct cpu_info *info)
{
    struct registers32 regs = {0};

    memset(info, 0, sizeof(*info));

    cpuid(0x0, 0, &regs);

    strncpy(info->vendor + 0, bit_str(regs.ebx), 4);
    strncpy(info->vendor + 4, bit_str(regs.edx), 4);
    strncpy(info->vendor + 8, bit_str(regs.ecx), 4);

    info->is_intel = (strstr(info->vendor, "Intel") != NULL);

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

    info->stepping = signature.stepping;
    info->model = signature.model;
    info->family = signature.family;
    info->ext_model = signature.ext_model;
    info->ext_family = signature.ext_family;

    info->features[F_SSE3] = bit(regs.ecx, 0);
    info->features[F_PCLMULQDQ] = bit(regs.ecx, 1);
    info->features[F_MONITOR] = bit(regs.ecx, 3);
    info->features[F_SSSE3] = bit(regs.ecx, 9);
    info->features[F_FMA] = bit(regs.ecx, 12);
    info->features[F_CMPXCHG16B] = bit(regs.ecx, 13);
    info->features[F_SSE4_1] = bit(regs.ecx, 19);
    info->features[F_SSE4_2] = bit(regs.ecx, 20);
    info->features[F_MOVBE] = bit(regs.ecx, 22);
    info->features[F_POPCNT] = bit(regs.ecx, 23);
    info->features[F_AES] = bit(regs.ecx, 25);
    info->features[F_XSAVE] = bit(regs.ecx, 26);
    info->features[F_OSXSAVE] = bit(regs.ecx, 27);
    info->features[F_AVX] = bit(regs.ecx, 28);
    info->features[F_F16C] = bit(regs.ecx, 29);
    info->features[F_RDRAND] = bit(regs.ecx, 30);

    info->features[F_MSR] = bit(regs.edx, 5);
    info->features[F_CX8] = bit(regs.edx, 8);
    info->features[F_SEP] = bit(regs.edx, 11);
    info->features[F_CMOV] = bit(regs.edx, 15);
    info->features[F_CLFSH] = bit(regs.edx, 19);
    info->features[F_MMX] = bit(regs.edx, 23);
    info->features[F_FXSR] = bit(regs.edx, 24);
    info->features[F_SSE] = bit(regs.edx, 25);
    info->features[F_SSE2] = bit(regs.edx, 26);
    info->features[F_HTT] = bit(regs.edx, 28);

    info->features[F_PREFETCHWT1] = bit(regs.ecx, 0);

    info->threads = (regs.ebx >> 16) & 0xFF;

    if (info->is_intel)
    {
        cpuid(0x4, 0, &regs);

        info->cores = ((regs.eax >> 26) & 0x3F) + 1;
    }
    else
    {
        cpuid(0x80000008, 0, &regs);

        info->cores = (regs.ecx & 0xFF) + 1;

        if (info->features[F_HTT])
            info->cores /= 2;
    }

    cpuid(0x7, 0, &regs);

    info->features[F_FSGSBASE] = bit(regs.ebx, 0);
    info->features[F_BMI1] = bit(regs.ebx, 3);
    info->features[F_HLE] = info->is_intel && bit(regs.ebx, 4);
    info->features[F_AVX2] = bit(regs.ebx, 5);
    info->features[F_BMI2] = bit(regs.ebx, 8);
    info->features[F_ERMS] = bit(regs.ebx, 9);
    info->features[F_INVPCID] = bit(regs.ebx, 10);
    info->features[F_RTM] = info->is_intel && bit(regs.ebx, 11);
    info->features[F_RDSEED] = bit(regs.ebx, 18);
    info->features[F_ADX] = bit(regs.ebx, 19);
    info->features[F_SHA] = bit(regs.ebx, 29);

    cpuid(0x80000001, 0, &regs);

    info->features[F_LAHF] = bit(regs.ecx, 0);
    info->features[F_LZCNT] = info->is_intel && bit(regs.ecx, 5);
    info->features[F_ABM] = !info->is_intel && bit(regs.ecx, 5);
    info->features[F_SSE4a] = !info->is_intel && bit(regs.ecx, 6);
    info->features[F_XOP] = !info->is_intel && bit(regs.ecx, 11);
    info->features[F_TBM] = !info->is_intel && bit(regs.ecx, 21);

    info->features[F_TBM] = info->is_intel && bit(regs.edx, 11);
    info->features[F_MMXEXT] = !info->is_intel && bit(regs.edx, 22);
    info->features[F_RDTSCP] = info->is_intel && bit(regs.edx, 27);
    info->features[F_3DNOWEXT] = !info->is_intel && bit(regs.edx, 30);
    info->features[F_3DNOW] = !info->is_intel && bit(regs.edx, 31);

    for (int i = 0x80000002, offset = 0; i <= 0x80000004; i++)
    {
        cpuid(i, 0, &regs);

        strncpy(info->name + offset + 0,  bit_str(regs.eax), 4);
        strncpy(info->name + offset + 4,  bit_str(regs.ebx), 4);
        strncpy(info->name + offset + 8,  bit_str(regs.ecx), 4);
        strncpy(info->name + offset + 12, bit_str(regs.edx), 4);

        offset += 16;
    }

    str_trimr(info->name);

    cpu_cache_update(info->caches, info->is_intel);
}

void cpu_features_str(const struct cpu_info *info,
                      string dest, size_t size)
{
    string buffer = malloc(BUFFER_SIZE);
    size_t len = 0;

    memset(buffer, 0, BUFFER_SIZE);

    for (int i = 0; i < F_TOTAL; i++)
    {
        if (info->features[i])
        {
            switch (i)
            {
            case F_MMX:
                if (info->features[F_MMXEXT])
                    continue;
                break;

            case F_3DNOW:
                if (info->features[F_3DNOWEXT])
                    continue;
                break;
            }

            const string feature = feature_names[i];
            size_t feature_len = s_strlen(feature);

            size_t new_size = len + feature_len + 2;

            if (new_size > BUFFER_SIZE)
            {
                string result = realloc(buffer, new_size);

                if (result) buffer = result;
                else goto err;
            }

            strncpy(buffer + len, feature, feature_len);
            len += feature_len;

            strncpy(buffer + len, ", ", 2);
            len += 2;
        }
    }

err:
    buffer[len - 2] = '\0';

    strncpy(dest, buffer, size);
    free(buffer);
}
