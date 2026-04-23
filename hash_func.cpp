#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stddef.h>

#define CRC32_POLYNOMIAL 0xEDB88320L

typedef unsigned long hash_t;


hash_t const_hash(char* /*str*/)
{
    return 1;
}

hash_t first_char_hash(char *str)
{
    return (hash_t)(str[0]);
}

hash_t len_hash(char *str)
{
    return strlen(str);
}

hash_t checksum_hash(char *str)
{
    hash_t checksum = 0;
    size_t len =  strlen(str);
    for (size_t i = 0; i < len; i++)
    {
        checksum += (hash_t)(str[i]);
    }

    return checksum;
}

hash_t roll_hash(char *str)
{
    hash_t new_hash = 0;
    while (*str != '\0')
    {
        new_hash = (new_hash << 5) - new_hash + (hash_t)(*str++);
    }

    return new_hash;
}

hash_t crc32_hash(char* str)
{
    if (str == NULL) return 0;
    
    uint32_t crc = 0xFFFFFFFF;
    
    for (int i = 0; str[i] != '\0'; i++)
    {
        uint8_t byte = (uint8_t)str[i];
        crc ^= byte;
        
        for (int bit = 0; bit < 8; bit++)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ CRC32_POLYNOMIAL;
            else
                crc = (crc >> 1);
        }
    }
    
    return (hash_t)(~crc);
}