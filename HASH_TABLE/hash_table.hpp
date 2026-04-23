#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <ctype.h>
#include "vector.hpp"

#define HT_SIZE 4219


typedef unsigned long hash_t;

enum ht_err_t
{
    HT_SUCCESS = 0,
    HT_ERROR   = 1
};

template <typename ht_elem_t>
struct ht_t
{
    int               is_used;
    vec_t<ht_elem_t>* vec;
};


template <typename ht_elem_t>
ht_err_t ht_init(ht_t<ht_elem_t> *ht);

template <typename ht_elem_t>
ht_err_t ht_free(ht_t<ht_elem_t> *ht);

template <typename ht_elem_t>
ht_elem_t* ht_find(ht_t<ht_elem_t> *ht, ht_elem_t target, hash_t (*hash_func)(ht_elem_t), bool (*equal_func)(ht_elem_t, ht_elem_t));

template <typename ht_elem_t>
ht_err_t ht_insert(ht_t<ht_elem_t> *ht, ht_elem_t item, hash_t (*hash_func)(ht_elem_t), bool (*equal_func)(ht_elem_t, ht_elem_t));

template <typename ht_elem_t>
ht_err_t ht_remove(ht_t<ht_elem_t> *ht, ht_elem_t item, hash_t (*hash_func)(ht_elem_t), bool (*equal_func)(ht_elem_t, ht_elem_t));

// template <typename ht_elem_t>
// void ht_dump(ht_t<ht_elem_t> *ht, void (*print_elem)(ht_elem_t));

#include "ht_func.hpp"


#endif