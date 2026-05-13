#include "hash_table.hpp"
#include <time.h>

#define SOURCE_FILE "src/LordOfRings.txt"
#define TEST_FILE   "src/Hobbit.txt"

hash_t crc32_hash(ht_entry_t* bucket);               

char* load_words_to_vec(FILE *stream, vec_t<char*> *words);
ht_err_t fill_ht(ht_t<ht_entry_t*> *ht, vec_t<char*> *words, ht_entry_t *buckets);
ht_err_t analyze_all_hashes(ht_t<ht_entry_t*> *ht, vec_t<char*> *words);

void benchmark_find(ht_t<ht_entry_t*> *ht, vec_t<char*> *words);


int main()
{
    ht_t<ht_entry_t*> *ht = (ht_t<ht_entry_t*>*)calloc_ex(HT_SIZE, sizeof(ht_t<ht_entry_t*>));
    if (ht == nullptr) { return -1; }
    ht_init<ht_entry_t*>(ht);

    vec_t<char*> *words = (vec_t<char*>*)calloc_ex(MIN_VEC_CAP, sizeof(vec_t<char*>));
    if (words == nullptr) { return -1; }
    
    FILE *src_file = fopen(SOURCE_FILE, "r");
    vec_init<char*>(words, MIN_VEC_CAP);
    char *src_buffer = load_words_to_vec(src_file, words);
    fclose(src_file);
    
    ht_entry_t *buckets = (ht_entry_t*)calloc_ex(words->size, sizeof(ht_entry_t));
    if (buckets == nullptr) { return -1; }
        
    fill_ht(ht, words, buckets);
    vec_free<char*>(words);

    FILE *test_file = fopen(TEST_FILE, "r");
    vec_init<char*>(words, MIN_VEC_CAP);
    char *test_buffer = load_words_to_vec(test_file, words);
    fclose(test_file);

    benchmark_find(ht, words);

    vec_free<char*>(words);
    free(words);
    free(src_buffer);
    free(test_buffer);

    free(buckets);
    ht_free<ht_entry_t*>(ht);
    free(ht);

    return 0;
}


ht_err_t fill_ht(ht_t<ht_entry_t*> *ht, vec_t<char*> *words, ht_entry_t *buckets)
{
    size_t words_size = words->size;
    ht_entry_t *cur_bucket = nullptr;

    for (size_t i = 0; i < words_size; i++)
    {
        char *word = nullptr;
        vec_pop_back<char*>(words, &word);

        cur_bucket = &buckets[i];

        cur_bucket->word = word;
        cur_bucket->hash = HASH_FUNC(cur_bucket);

        ht_insert<ht_entry_t*, EQUAL_FUNC>(ht, cur_bucket, HASH_FUNC);
    }

    return HT_SUCCESS;
}