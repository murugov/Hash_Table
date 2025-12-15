#include "hash_table.hpp"


template <typename htElem_T>
htErr_t htInit(ht_t<htElem_T> *hash_table, const char *name, const char *file, const char *func, int line)
{
    ON_DEBUG( if (IS_BAD_PTR(hash_table)) { LOG(ERROR, "STK_BAD_STK_PTR"); return HT_ERROR; } )
    
    (*hash_table).id.name = name;
    (*hash_table).id.file = file;
    (*hash_table).id.func = func;
    (*hash_table).id.line = line;

    ON_DEBUG( LOG(DEBUG, "Created id for hash-table \"%s\"", hash_table->id.name); )
    return HT_SUCCESS;
}


template <typename htElem_T>
htErr_t htCtor(ht_t<htElem_T> *hash_table)
{
    ON_DEBUG( if (IS_BAD_PTR(hash_table)) { LOG(ERROR, "BAD_HT_PTR"); return HT_ERROR; } )
    
    hash_table->table = (table_t<htElem_T>*)calloc(HT_SIZE, sizeof(table_t<htElem_T>));
    if (IS_BAD_PTR(hash_table->table)) { return HT_ERROR; }
    
    for (int i = 0; i < HT_SIZE; ++i)
    {
        hash_table->table[i].is_used = 0;
        
        stk_t<htElem_T> *stk = NULL;
        STACK_CTOR(&stk, INIT_NUM_COL);
        if (stk->error != STK_NO_ERRORS) { return HT_ERROR; }
        hash_table->table[i].stk = stk;
    }
    
    return HT_SUCCESS;
}

template <typename htElem_T>
htErr_t htDtor(ht_t<htElem_T> *hash_table)
{
    ON_DEBUG( if (IS_BAD_PTR(hash_table)) { LOG(ERROR, "BAD_HT_PTR"); return HT_ERROR; } )

    for (int i = 0; i < HT_SIZE; ++i)
    {
        (hash_table[i].table)->is_used = 0;
        STACK_DTOR((hash_table[i].table)->stk);
        ON_DEBUG( if ((hash_table[i].table)->stk->error != STK_DESTROYED) { LOG(ERROR, "HT_DTOR_FAIL"); return HT_ERROR; } )
        free((hash_table[i].table)->stk);
    }

    free(hash_table);
}


template <typename htElem_T>
htElem_T *htFind(ht_t<htElem_T> *hash_table, htElem_T *target, hash_t (*hash_func)(const char*))
{
    ON_DEBUG( if(IS_BAD_PTR(hash_table) || IS_BAD_PTR(target) || IS_BAD_PTR(hash_func)) return NULL; )

    const char* char_target = reinterpret_cast<const char*>(target);                // for now there is a type "char*", then there will be a hash_func for different types
    hash_t hash_target = hash_func(char_target) & (HT_SIZE - 1);

    if (hash_table[hash_target].is_used == 0) return NULL;
    
    stk_t<htElem_T> *stk = hash_table[hash_target].stk;

    for (ssize_t i = 0; i < stk->size; ++i)
    {
        const char* current_elem = reinterpret_cast<const char*>(stk->data[i]);
        if (strncmp(char_target, current_elem, MAX_LEN_STR_FOR_HASH) == 0)
            return stk->data[i];
    }

    return NULL;
}


template <typename htElem_T>
htErr_t htInsert(ht_t<htElem_T> *hash_table, htElem_T *item, hash_t (*hash_func)(const char*))
{
    ON_DEBUG( if(IS_BAD_PTR(hash_table) || IS_BAD_PTR(item)) return HT_ERROR; )

    const char* char_item = reinterpret_cast<const char*>(item);
    hash_t hash_item = hash_func(char_item) & (HT_SIZE - 1);

    hash_table[hash_item].is_used = 1;
    StackPush(hash_table[hash_item].stk, item);

    return HT_SUCCESS;
}


template <typename htElem_T>
htErr_t htRemove(ht_t<htElem_T> *hash_table, htElem_T *item, hash_t (*hash_func)(const char*))
{
    ON_DEBUG( if(IS_BAD_PTR(hash_table) || IS_BAD_PTR(item)) return HT_ERROR; )

    const char* char_item = reinterpret_cast<const char*>(item);
    hash_t hash_item = hash_func(char_item) & (HT_SIZE - 1);

    if (hash_table[hash_item] == 0) return HT_ERROR;

    stk_t<htElem_T> *stk_ret = NULL;
    STACK_CTOR(stk_ret, INIT_NUM_COL);

    htElem_T *current_elem = NULL;
    while (strncmp(char_item, current_elem, MAX_LEN_STR_FOR_HASH) != 0)
    {
        StackPop(hash_table[hash_item].stk, current_elem);
        StackPush(stk_ret, *current_elem);
    }

    while (stk_ret->size > 0)
    {
        StackPop(stk_ret, current_elem);     
        StackPush(hash_table[hash_item].stk, *current_elem);
    }

    STACK_DTOR(stk_ret);

    if (hash_table[hash_item].stk->size == 0) hash_table[hash_item].is_used = 0;

    return HT_SUCCESS;
}