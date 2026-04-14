#include "hash_table.hpp"

typedef unsigned long hash_t;               // remove
hash_t hash_func_str(char* str);
bool equal_func_str(char* str1, char* str2);


int main()
{
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

    return 0;
}


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
}