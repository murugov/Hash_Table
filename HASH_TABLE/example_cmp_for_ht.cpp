#include <stdio.h>
#include <string.h>

struct var_t
{
    const char*  name;
    double 		 value;
};

const char *htVarToStr(const void *ht_elem)
{
   const char *var_name = ((var_t*)ht_elem)->name;

    return var_name;
}