#include "hash_table.hpp"


int main()
{
    LogFileOpener(PATH_TO_LOGFILE);

    ht_t<var_t*> *hash_table_1 = (ht_t<var_t*>*)calloc(1, sizeof(ht_t<var_t*>));
    HT_CTOR(hash_table_1);

    var_t *item = (var_t*)calloc(1, sizeof(var_t));
    item->name = "x";
    htInsert(hash_table_1, &item, htVarToStr);
    // htInsert(hash_table_1, "y", htStringsEqual);
    // htInsert(hash_table_1, "z", htStringsEqual);

    // htRemove(hash_table_1, "z", htStringsEqual);

    var_t *item_2 = (var_t*)calloc(1, sizeof(var_t));
    item_2->name = "y";
    printf("%p\n", htFind(hash_table_1, &item_2, htVarToStr));

    HT_DTOR(hash_table_1);
    free(hash_table_1);

    LogFileCloser();
    return 0;
}