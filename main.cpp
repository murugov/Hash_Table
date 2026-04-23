#include "hash_table.hpp"

#define SOURCE_FILE "src/LordOfRings.txt"

hash_t const_hash(char *str);
hash_t first_char_hash(char *str);
hash_t len_hash(char *str);
hash_t checksum_hash(char *str);
hash_t roll_hash(char *str);
hash_t crc32_hash(char* str);

bool equal_func_str(char* str1, char* str2);
// inline void print_string(char* str);
char* load_words_to_vec(FILE *stream, vec_t<char*> *lines);
ht_err_t fill_ht(ht_t<char*> *ht, vec_t<char*> *words);
int gen_hist(int ht_size, int *arr);


int main()
{
    ht_t<char*> *ht = (ht_t<char*>*)calloc(HT_SIZE, sizeof(ht_t<char*>));
    if (ht == NULL) { return 1; }
    ht_init(ht);

    vec_t<char*> *words = (vec_t<char*>*)calloc(1, sizeof(vec_t<char*>));
    if (words == NULL) { return 1; }
    vec_init(words, MIN_VEC_CAP);

    FILE *file = fopen(SOURCE_FILE, "r");
    char *buffer = load_words_to_vec(file, words);
    fclose(file);

    fill_ht(ht, words);

    int *arr = (int*)calloc(HT_SIZE, sizeof(int));
    if (arr == NULL) { return 1; }

    for (int i = 0; i < HT_SIZE; i++) 
    {
        if (ht[i].vec == NULL)
        {
            arr[i] = 0;
        }
        else
        {
            arr[i] = (int)(ht[i].vec)->size;
        }
    }

    gen_hist(HT_SIZE, arr);

    free(arr);
    vec_free(words);
    free(buffer);
    free(words);
    ht_free(ht);
    free(ht);

    return 0;
}

inline bool equal_func_str(char* str1, char* str2)
{
    return (strcmp(str1, str2) == 0);
}

// inline void print_string(char* str)
// {
//     if (str != NULL) { printf("%s\n", str); } 
//     else { printf("(null)\n"); }
// }


ht_err_t fill_ht(ht_t<char*> *ht, vec_t<char*> *words)
{
    while (words->size > 0)
    {
        char *word = NULL;
        vec_pop_back(words, &word);
        ht_insert(ht, word, roll_hash, equal_func_str);
    }

    return HT_SUCCESS;
}