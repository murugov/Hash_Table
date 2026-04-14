#ifndef HT_FUNC_HPP
#define HT_FUNC_HPP

#include "hash_table.hpp"


template <typename ht_elem_t>
ht_err_t ht_init(ht_t<ht_elem_t> *ht)
{    
    for (int i = 0; i < HT_SIZE; i++)
    {
        ht[i].is_used = 0;
        ht[i].vec     = NULL;
    }
    
    return HT_SUCCESS;
}


template <typename ht_elem_t>
ht_err_t ht_free(ht_t<ht_elem_t> *ht)
{
    if (ht == NULL) { return HT_SUCCESS; }

    for (int i = 0; i < HT_SIZE; i++)
    {
        if (ht[i].vec != NULL)
        {
            vec_free(ht[i].vec);
            free(ht[i].vec);
            ht[i].vec = NULL;
        }
        ht[i].is_used = 0;
    }

    return HT_SUCCESS;
}


template <typename ht_elem_t>
ht_elem_t* ht_find(ht_t<ht_elem_t> *ht, ht_elem_t target, hash_t (*hash_func)(ht_elem_t), bool (*equal_func)(ht_elem_t, ht_elem_t))
{
    hash_t target_hash = hash_func(target) & (HT_SIZE - 1);

    if (ht[target_hash].is_used == 0) { return NULL; }
    
    vec_t<ht_elem_t> *vec = ht[target_hash].vec;
    if (vec == NULL) { return NULL; }

    for (size_t i = 0; i < vec->size; i++)
    {
        if (equal_func(vec->data[i], target))
        {
            return &vec->data[i];
        }
    }

    return NULL;
}


template <typename ht_elem_t>
ht_err_t ht_insert(ht_t<ht_elem_t> *ht, ht_elem_t item, hash_t (*hash_func)(ht_elem_t), bool (*equal_func)(ht_elem_t, ht_elem_t))
{
    hash_t item_hash = hash_func(item) & (HT_SIZE - 1);

    if (ht[item_hash].is_used == 0)
    {
        vec_t<ht_elem_t> *new_vec = (vec_t<ht_elem_t>*)calloc(1, sizeof(vec_t<ht_elem_t>));
        if (new_vec == NULL) { return HT_ERROR; }

        vec_init(new_vec, MIN_VEC_CAP);
        ht[item_hash].vec     = new_vec;
        ht[item_hash].is_used = 1;
    }
    else
    {
        vec_t<ht_elem_t> *vec = ht[item_hash].vec;
        for (size_t i = 0; i < vec->size; i++)
        {
            if (equal_func(vec->data[i], item))
            {
                return HT_SUCCESS;
            }
        }

    }

    vec_push_back(ht[item_hash].vec, item);

    return HT_SUCCESS;
}


template <typename ht_elem_t>
ht_err_t ht_remove(ht_t<ht_elem_t> *ht, ht_elem_t item, hash_t (*hash_func)(ht_elem_t), bool (*equal_func)(ht_elem_t, ht_elem_t))
{
    hash_t item_hash = hash_func(item) & (HT_SIZE - 1);
    
    if (ht[item_hash].is_used == 0) { return HT_ERROR; }
    
    vec_t<ht_elem_t> *vec = ht[item_hash].vec;
    if (vec == NULL) { return HT_ERROR; }
    
    for (size_t i = 0; i < vec->size; i++)
    {
        if (equal_func(vec->data[i], item))
        {
            vec_remove(vec, i);
            return HT_SUCCESS;
        }
    }
    
    return HT_ERROR;
}


#endif