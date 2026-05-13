#ifndef HT_FUNC_HPP
#define HT_FUNC_HPP

#include "hash_table.hpp"


template <typename ht_elem_t>
ht_err_t ht_init(ht_t<ht_elem_t> * __restrict ht)
{    
    for (int i = 0; i < HT_SIZE; i++)
    {
        ht[i].is_used = 0;
        ht[i].vec     = nullptr;
    }
    
    return HT_SUCCESS;
}


template <typename ht_elem_t>
ht_err_t ht_free(ht_t<ht_elem_t> * __restrict ht)
{
    if (ht == nullptr) { return HT_SUCCESS; }

    for (int i = 0; i < HT_SIZE; i++)
    {
        if (ht[i].vec != nullptr)
        {
            vec_free(ht[i].vec);
            free(ht[i].vec);
            ht[i].vec = nullptr;
        }
        ht[i].is_used = 0;
    }

    return HT_SUCCESS;
}

template <typename ht_elem_t>
inline ht_err_t ht_clear(ht_t<ht_elem_t> * __restrict ht)
{
    for (int i = 0; i < HT_SIZE; i++)
    {
        ht[i].is_used = 0;
        vec_free(ht[i].vec);
        ht[i].vec     = nullptr;
    }

    return HT_SUCCESS;
}


template <typename ht_elem_t, bool (*equal_func)(const ht_entry_t*, const ht_entry_t*)>
__attribute__ ((noinline, noclone))
ht_elem_t* ht_find(ht_t<ht_elem_t> * __restrict ht, ht_elem_t target, hash_t (*hash_func)(ht_elem_t))
{
    // __asm__ volatile (
    //     "prefetcht0 256(%[ptr])\n\t"
    //     :
    //     : [ptr] "r" (target->word)
    //     : "memory"
    // );

    const hash_t target_index = hash_func(target) % (HT_SIZE - 1);    
    if (!ht[target_index].is_used) { return nullptr; }
    
    vec_t<ht_elem_t> * __restrict vec = ht[target_index].vec;
    if (vec == nullptr) { return nullptr; }

    ht_elem_t* __restrict data_ptr = vec->data;
    const size_t vec_size = vec->size;

    for (size_t i = 0; i < vec_size; i++)
    {
        if (equal_func(data_ptr[i], target))
        {
            return &data_ptr[i];
        }
    }

    // __asm__ volatile (
    //     "vmovdqu (%[t_word]), %%ymm0\n\t"
        
    //     "1:\n\t"
    //     "cmpq %[t_hash], (%[ptr])\n\t"
    //     "jne 3f\n\t"
        
    //     "movq 8(%[ptr]), %%rcx\n\t"
    //     "vmovdqu (%%rcx), %%ymm1\n\t"
    //     "vpcmpeqb %%ymm0, %%ymm1, %%ymm1\n\t"
    //     "vpmovmskb %%ymm1, %%eax\n\t"
    //     "cmpl $0xffffffff, %%eax\n\t"
    //     "je 4f\n\t"
        
    //     "3:\n\t"
    //     "addq $16, %[ptr]\n\t"
    //     "decq %[count]\n\t"
    //     "jnz 1b\n\t"
        
    //     "xorq %[res], %[res]\n\t"
    //     "jmp 2f\n\t"

    //     "4:\n\t"
    //     "movq %[ptr], %[res]\n\t"

    //     "2:\n\t"
    //     "vzeroupper\n\t"
    //     : [res] "=a" (found_ptr), [ptr] "+r" (data_ptr), [count] "+r" (items_left)
    //     : [t_hash] "r" (target_hash), [t_word] "r" (target_word)
    //     : "rcx", "ymm0", "ymm1", "cc", "memory"
    // );


    return nullptr;
}


template <typename ht_elem_t, bool (*equal_func)(const ht_entry_t*, const ht_entry_t*)>
ht_err_t ht_insert(ht_t<ht_elem_t> * __restrict ht, ht_elem_t item, hash_t (*hash_func)(ht_elem_t))
{
    const hash_t item_index = hash_func(item) % (HT_SIZE - 1);
    if (ht[item_index].is_used == 0)
    {
        vec_t<ht_elem_t> * __restrict new_vec = (vec_t<ht_elem_t>*)calloc_ex(1, sizeof(vec_t<ht_elem_t>));
        if (new_vec == nullptr) { return HT_ERROR; }

        vec_init(new_vec, MIN_VEC_CAP);
        ht[item_index].vec     = new_vec;
        ht[item_index].is_used = 1;
    }
    else
    {
        ht_elem_t* __restrict data_ptr = (ht[item_index].vec)->data;
        const size_t vec_size = (ht[item_index].vec)->size;

        for (size_t i = 0; i < vec_size; i++)
        {
            if (equal_func(data_ptr[i], item))
            {
                return HT_SUCCESS;
            }
        }

    }

    vec_push_back(ht[item_index].vec, item);

    return HT_SUCCESS;
}


template <typename ht_elem_t, bool (*equal_func)(const ht_entry_t*, const ht_entry_t*)>
ht_err_t ht_remove(ht_t<ht_elem_t> * __restrict ht, ht_elem_t item, hash_t (*hash_func)(ht_elem_t))
{
    const hash_t item_index = hash_func(item) % (HT_SIZE - 1);

    const ht_t<ht_elem_t> ht_bucket = ht[item_index];
    if (ht_bucket.is_used == 0) { return HT_ERROR; }
    
    vec_t<ht_elem_t> * __restrict vec = ht_bucket.vec;
    if (vec == nullptr) { return HT_ERROR; }
    
    const size_t vec_size = vec->size;

    for (size_t i = 0; i < vec_size; i++)
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