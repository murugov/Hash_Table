#include "hash_table.hpp"


#define CYCLES 500

hash_t crc32_hash(ht_entry_t* bucket);
ht_err_t benchmark_find(ht_t<ht_entry_t*> *ht, vec_t<char*> *words);


ht_err_t benchmark_find(ht_t<ht_entry_t*> *ht, vec_t<char*> *words)
{
    size_t words_size = words->size;

    ht_entry_t *buckets = (ht_entry_t*)calloc_ex(words_size, sizeof(ht_entry_t));
    if (nullptr) { return HT_ERROR; }

    ht_entry_t *cur_bucket = nullptr;
    char *cur_word = nullptr;

    for (size_t i = 0; i < words_size; i++)
    {
        cur_bucket = &buckets[i];
        cur_word = words->data[i];

        cur_bucket->hash = 0;
        cur_bucket->word = cur_word;
    }

    
    size_t sum_clocks = 0;
    
    for (size_t j = 0; j < CYCLES; j++)
    {    
        uint64_t start = __rdtsc();
        for (size_t i = 0; i < words_size; i++)
        {       
            volatile ht_entry_t **found_addr = const_cast<volatile ht_entry_t**>(ht_find<ht_entry_t*, EQUAL_FUNC>(ht, &buckets[i], HASH_FUNC));
        }
        uint64_t end = __rdtsc();

        sum_clocks += end - start;
    }

    printf("CPC: %zu\n", sum_clocks / CYCLES);

    free(buckets);
    
    return HT_SUCCESS;
}