#include <stdio.h>
#include <string.h>


int htStringsEqual(const void *ht_elem, const char *item)
{
    const char *str_in_table = (const char*)ht_elem;
    
    if (str_in_table == NULL && item == NULL) return 1;
    if (str_in_table == NULL || item == NULL) return 0;
    
    return strcmp(str_in_table, item) == 0;
}