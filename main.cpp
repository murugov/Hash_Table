#include "hash_table.hpp"


int main()
{
    LogFileOpener(PATH_TO_LOGFILE);

    ht_t<const char*> *hash_table_1 = (ht_t<const char*>*)calloc(1, sizeof(ht_t<const char*>));
    HT_CTOR(hash_table_1);

    htInsert(hash_table_1, "x", htStringsEqual);
    htInsert(hash_table_1, "y", htStringsEqual);
    htInsert(hash_table_1, "z", htStringsEqual);

    htRemove(hash_table_1, "z", htStringsEqual);

    printf("%p\n", htFind(hash_table_1, "y", htStringsEqual));

    HT_DTOR(hash_table_1);
    free(hash_table_1);

    LogFileCloser();
    return 0;
}