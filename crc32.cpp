#include <stdint.h>
#include <stddef.h>

static uint32_t crc32_table[256];

static void init_crc32_table(void)
{
    uint32_t polynomial = 0xEDB88320L;
    
    for (uint32_t i = 0; i < 256; i++)
    {
        uint32_t crc = i;
        for (int j = 0; j < 8; j++)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ polynomial;
            else
                crc >>= 1;
        }
        crc32_table[i] = crc;
    }
}

uint32_t crc32_hash(char* str)
{
    if (str == NULL) return 0;
    
    static int table_initialized = 0;
    if (!table_initialized)
    {
        init_crc32_table();
        table_initialized = 1;
    }
    
    uint32_t crc = 0xFFFFFFFF;
    
    for (int i = 0; str[i] != '\0'; i++)
    {
        uint8_t byte = (uint8_t)str[i];
        crc = (crc >> 8) ^ crc32_table[(crc ^ byte) & 0xFF];
    }
    
    return ~crc;
}