#include "hardware/bios.h"

#if _DEBUG

typedef unsigned char byte;

struct dmi_header
{
    byte type;
    byte length;
    unsigned short handle;
};

static const char *dmi_string(const struct dmi_header *header,
                              byte pos)
{
    const char *str = (const char*) header;

    if (!pos) return "unknown";

    for (str += header->length; pos > 1 && *str; pos--)
        str += strlen(str) + 1;

    return str;
}

#if _WIN32

struct smbios_table
{
    byte entry;
    byte major_ver;
    byte minor_ver;
    byte revision;
    unsigned long length;
    byte data[];
};

void bios_identify(struct bios_info *info)
{
    size_t size, i = 0;

    struct smbios_table *table;
    
    if (size = GetSystemFirmwareTable('RSMB', 0, NULL, 0))
    {
        table = (struct smbios_table*) malloc(size);

        GetSystemFirmwareTable('RSMB', 0, table, size);
 
        byte *data = (byte*) table->data;

        while (1)
        {
            struct dmi_header *header = (struct dmi_header*) data;

            if (header->type == 0)
            {
                strncpy(info->vendor, dmi_string(header, data[0x4]),
                        BIOS_VEN_SIZE);
                
                strncpy(info->version, dmi_string(header, data[0x5]),
                        BIOS_VER_SIZE);
            }

            int off = header->length + 0x12 + 3;

            data += header->length + 0x12 + 3;
        }

        free(table);
    }
}

#else /* _WIN32 */
void bios_identify(struct bios_info *info)
{

}
#endif

#endif /* _DEBUG */