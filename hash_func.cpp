#include <stdint.h>
#include "hash_table.hpp"

#define CRC32_POLYNOMIAL 0xEDB88320L

hash_t const_hash(ht_entry_t* /*bucket*/);
hash_t first_char_hash(ht_entry_t *bucket);
hash_t len_hash(ht_entry_t *bucket);
hash_t checksum_hash(ht_entry_t *bucket);
hash_t roll_hash(ht_entry_t *bucket);
hash_t crc32_hash(ht_entry_t* bucket);


hash_t const_hash(ht_entry_t* /*bucket*/)
{    
    return 0;
}

hash_t first_char_hash(ht_entry_t *bucket)
{
    return (hash_t)(bucket->word[0]);
}

hash_t len_hash(ht_entry_t *bucket)
{
    return strlen(bucket->word);
}

hash_t checksum_hash(ht_entry_t *bucket)
{
    hash_t checksum = 0;
    for (size_t i = 0; bucket->word[i] != '\0'; i++)
    {
        checksum += (hash_t)(bucket->word[i]);
    }

    return checksum;
}

hash_t roll_hash(ht_entry_t *bucket)
{
    hash_t new_hash = 0;
    for (size_t i = 0; bucket->word[i] != '\0'; i++)
    {
        new_hash = (new_hash << 5) - new_hash + (hash_t)(bucket->word[i]);
    }

    return new_hash;
}

// TODO: rol

// hash_t crc32_hash(ht_entry_t *bucket)
// {    
//     char *word = bucket->word;
//     const uint8_t *byte = (const uint8_t *)word;
//     hash_t crc = 0xFFFFFFFF;

//     for (size_t i = 0; word[i] != '\0'; i++)
//     {
//         crc ^= byte[i];
//         for (int j = 0; j < 8; j++)
//         {
//             crc >>= 1;
//             if (crc & 1)
//             {
//                 crc ^= CRC32_POLYNOMIAL;
//             }
//         }
//     }

//     return ~crc; 
// }


hash_t crc32_hash(ht_entry_t *bucket)
{
    const uint8_t *ptr = (const uint8_t *)bucket->word;    
    uint32_t crc = 0xFFFFFFFF;

    crc = (uint32_t)_mm_crc32_u64(crc, *(const uint64_t*)(ptr + 0));
    crc = (uint32_t)_mm_crc32_u64(crc, *(const uint64_t*)(ptr + 8));
    crc = (uint32_t)_mm_crc32_u64(crc, *(const uint64_t*)(ptr + 16));
    crc = (uint32_t)_mm_crc32_u64(crc, *(const uint64_t*)(ptr + 24));

    bucket->hash = (hash_t)~crc;
    return (hash_t)~crc;
}
