#include "hash_table.hpp"


int main()
{
    LogFileOpener(PATH_TO_LOGFILE);

    ht_t *hash_table_1 = (ht_t*)calloc(1, sizeof(ht_t));
    HT_CTOR(hash_table_1);

    htInsert(hash_table_1, "x");
    htInsert(hash_table_1, "y");
    htInsert(hash_table_1, "z");

    htRemove(hash_table_1, "z");

    printf("%p\n", htFind(hash_table_1, "z"));

    HT_DTOR(hash_table_1);
    LogFileCloser();
    return 0;
}