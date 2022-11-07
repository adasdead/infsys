#ifndef HARDWARE_BIOS_H
#define HARDWARE_BIOS_H

#include "infsys.h"

#define BIOS_VEN_SIZE 64
#define BIOS_VER_SIZE 12

struct bios_info
{
    char vendor[BIOS_VEN_SIZE];
    char version[BIOS_VER_SIZE];
};

void bios_identify(struct bios_info *info);

#endif /* HARDWARE_BIOS_H */