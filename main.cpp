#include "hash_table.hpp"


int main()
{
    LogFileOpener(PATH_TO_LOGFILE);

    ht_t<char*> *hash_table_1 = NULL;
    HT_CTOR(hash_table_1);

    HT_DTOR(hash_table_1);
    LogFileCloser();
    return 0;
}