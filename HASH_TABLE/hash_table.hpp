#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <ctype.h>
#include <time.h>
#include <x86intrin.h>
#include <smmintrin.h>
#include "vector.hpp"

#define HT_SIZE 4219

#define HASH_FUNC crc32_hash
#define EQUAL_FUNC equal_func_bucket

typedef unsigned long hash_t;


enum ht_err_t
{
    HT_SUCCESS = 0,
    HT_ERROR   = 1
};

template <typename ht_elem_t>
struct ht_t
{
    vec_t<ht_elem_t>* vec;
    size_t            is_used;
};

struct ht_entry_t
{
    hash_t hash;
    char*  word;
};

// extern "C" hash_t crc32_hash_asm(ht_entry_t *bucket);


// __attribute__ ((noinline, noclone, pure))
// inline bool equal_func_bucket(const ht_entry_t* bucket1, const ht_entry_t* bucket2)
// {
//     return (bucket1->hash == bucket2->hash) && (strcmp(bucket1->word, bucket2->word) == 0);
// }

__attribute__ ((noinline, noclone, pure))
inline bool equal_func_bucket(const ht_entry_t* bucket1, const ht_entry_t* bucket2)
{
    if (bucket1->hash == bucket2->hash)
    {
        __m256i word1 = _mm256_load_si256((__m256i*)(bucket1->word));
        __m256i word2 = _mm256_load_si256((__m256i*)(bucket2->word));
        __m256i res = _mm256_xor_si256(word1, word2);

        return _mm256_testz_si256(res, res);
    }

    return false;
}


template <typename ht_elem_t>
ht_err_t ht_init(ht_t<ht_elem_t> * __restrict ht);

template <typename ht_elem_t>
ht_err_t ht_free(ht_t<ht_elem_t> * __restrict ht);

template <typename ht_elem_t>
inline ht_err_t ht_clear(ht_t<ht_elem_t> * __restrict ht);

template <typename ht_elem_t, bool (*equal_func)(const ht_entry_t*, const ht_entry_t*)>
ht_elem_t* ht_find(ht_t<ht_elem_t> * __restrict ht, ht_elem_t target, hash_t (*hash_func)(ht_elem_t));

template <typename ht_elem_t, bool (*equal_func)(const ht_entry_t*, const ht_entry_t*)>
ht_err_t ht_insert(ht_t<ht_elem_t> * __restrict ht, ht_elem_t item, hash_t (*hash_func)(ht_elem_t));

template <typename ht_elem_t, bool (*equal_func)(const ht_entry_t*, const ht_entry_t*)>
ht_err_t ht_remove(ht_t<ht_elem_t> * __restrict ht, ht_elem_t item, hash_t (*hash_func)(ht_elem_t));

#include "ht_func.hpp"


#endif