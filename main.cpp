#include "hash_table.hpp"

<<<<<<< HEAD
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
=======
typedef unsigned long hash_t;               // remove
hash_t hash_func_str(char* str);
bool equal_func_str(char* str1, char* str2);
>>>>>>> ce9a91af20a404497839faef230d97ce2a710d85


int main()
{
<<<<<<< HEAD
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
=======
    ht_t<char*> *ht_1 = (ht_t<char*>*)calloc(HT_SIZE, sizeof(ht_t<char*>));
    ht_init(ht_1);

    char *item1 = strdup("xui");
    char *item2 = strdup("pizda");
    char *item3 = strdup("eblan");

    ht_insert(ht_1, item1, hash_func_str, equal_func_str);
    ht_insert(ht_1, item2, hash_func_str, equal_func_str);
    ht_insert(ht_1, item3, hash_func_str, equal_func_str);

    ht_remove(ht_1, item2, hash_func_str, equal_func_str);

    printf("%p\n", ht_find(ht_1, item2, hash_func_str, equal_func_str));
    ht_free(ht_1);
    free(ht_1);
    free(item1);
    free(item2);
    free(item3);
>>>>>>> ce9a91af20a404497839faef230d97ce2a710d85

    return 0;
}

<<<<<<< HEAD
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
=======

hash_t hash_func_str(char* str)
{
    hash_t new_hash = 0;
    int renum = 0;
    while (renum < 8 && *str != '\0' && !isspace(*str))
    {
        new_hash = (new_hash << 5) - new_hash + (hash_t)(*str++);
        renum++;
    }

    return new_hash;
}


inline bool equal_func_str(char* str1, char* str2)         // inline
{
    return (strncmp(str1, str2, 8) == 0);
>>>>>>> ce9a91af20a404497839faef230d97ce2a710d85
}