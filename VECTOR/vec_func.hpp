#ifndef VEC_FUNC_HPP
#define VEC_FUNC_HPP

#include "vector.hpp"


template <typename vec_elem_t>
vec_err_t vec_init(vec_t<vec_elem_t> *vec, size_t cap)
{   
    if ((ssize_t)cap < 0) { return VEC_ERROR; }
    
    vec->cap  = cap;
    vec->size = 0;

    vec->data = (vec_elem_t*)calloc(vec->cap, sizeof(vec_elem_t));
    if (vec->data == NULL) { return VEC_ERROR; }

    return VEC_SUCCESS;
}

template <typename vec_elem_t>
vec_err_t vec_free(vec_t<vec_elem_t> *vec)
{
    if (vec == NULL) { return VEC_SUCCESS; }

    free(vec->data);
    vec->data = NULL;
    vec->size = 0;
    vec->cap  = 0;

    return VEC_SUCCESS;
}

template <typename vec_elem_t>
vec_err_t vec_push_back(vec_t<vec_elem_t> *vec, const vec_elem_t val)
{
    if (vec->size >= vec->cap)
    {
        if (vec_grow(vec) == VEC_ERROR) { return VEC_ERROR; }
    }

    vec->data[vec->size] = val;
    (vec->size)++;

    return VEC_SUCCESS;
}

template <typename vec_elem_t>
vec_err_t vec_push_front(vec_t<vec_elem_t> *vec, const vec_elem_t val)
{
    if (vec->size >= vec->cap)
    {
        if (vec_grow(vec) == VEC_ERROR) { return VEC_ERROR; }
    }

    if (vec->size > 0)
    {
        memmove((vec->data) + 1, vec->data, vec->size * sizeof(vec_elem_t));
    }
    
    vec->data[0] = val;
    vec->size++;

    return VEC_SUCCESS;
}

template <typename vec_elem_t>
vec_err_t vec_pop_back(vec_t<vec_elem_t> *vec, vec_elem_t *val)
{
    if (vec->size == 0) { return VEC_ERROR; }

    (vec->size)--;
    *val = vec->data[vec->size];
    vec->data[vec->size] = 0;

    size_t cap_div_by_2 = vec->cap >> 2;
    if (vec->size < cap_div_by_2 && cap_div_by_2 > MIN_VEC_CAP)
    {
        if (vec_shrink(vec) == VEC_ERROR) { return VEC_ERROR; }
    }

    return VEC_SUCCESS;
}

template <typename vec_elem_t>
vec_err_t vec_pop_front(vec_t<vec_elem_t> *vec, vec_elem_t *val)
{
    if (vec->size == 0) { return VEC_ERROR; }
    
    if (val) { *val = vec->data[0]; }
    
    if (vec->size > 1)
    {
        memmove(vec->data, vec->data + 1, (vec->size - 1) * sizeof(vec_elem_t));
    }
    
    vec->size--;
    vec->data[vec->size] = 0;
    
    size_t cap_div_by_2 = vec->cap >> 2;
    if (vec->size < cap_div_by_2 && cap_div_by_2 >= MIN_VEC_CAP)
    {
        if (vec_shrink(vec) == VEC_ERROR) { return VEC_ERROR; }
    }
    
    return VEC_SUCCESS;
}

template <typename vec_elem_t>
vec_err_t vec_clear(vec_t<vec_elem_t> *vec)
{
    memset(vec->data, 0, (vec->size) * sizeof(vec_elem_t));
    vec->size = 0;

    return VEC_SUCCESS;
}

template <typename vec_elem_t>
bool vec_empty(vec_t<vec_elem_t> *vec)
{
    if (vec->size == 0) { return true; }

    return false;
}

template <typename vec_elem_t>
vec_err_t vec_grow(vec_t<vec_elem_t> *vec)
{
    size_t new_cap = vec->cap << 1;
    
    vec_elem_t *new_data = (vec_elem_t*)realloc(vec->data, new_cap * sizeof(vec_elem_t));
    if(new_data == NULL) { return VEC_ERROR; }

    for (size_t i = vec->cap; i < new_cap; ++i) { new_data[i] = 0; }
    
    vec->cap  = new_cap;
    vec->data = new_data;

    return VEC_SUCCESS;
}

template <typename vec_elem_t>
vec_err_t vec_reserve(vec_t<vec_elem_t> *vec, const unsigned int new_cap)
{    
    vec_elem_t *new_data = (vec_elem_t*)realloc(vec->data, new_cap * sizeof(vec_elem_t));
    if(new_data == NULL) { return VEC_ERROR; }

    for (size_t i = vec->cap; i < new_cap; ++i) { new_data[i] = 0; }
    
    vec->cap  = new_cap;
    vec->data = new_data;

    return VEC_SUCCESS;
}

template <typename vec_elem_t>
vec_err_t vec_shrink(vec_t<vec_elem_t> *vec)
{
    size_t new_cap = vec->cap >> 2;
        
    vec_elem_t *new_data = (vec_elem_t*)realloc(vec->data, new_cap * sizeof(vec_elem_t));
    if(new_data == NULL) { return VEC_ERROR; }

    vec->cap  = new_cap;
    vec->data = new_data;

    return VEC_SUCCESS;
}

template <typename vec_elem_t>
vec_err_t vec_truncate(vec_t<vec_elem_t> *vec, const unsigned int new_cap)
{        
    vec_elem_t *new_data = (vec_elem_t*)realloc(vec->data, new_cap * sizeof(vec_elem_t));
    if(new_data == NULL) { return VEC_ERROR; }

    vec->cap  = new_cap;
    vec->data = new_data;

    return VEC_SUCCESS;
}

template <typename vec_elem_t>
vec_err_t vec_remove(vec_t<vec_elem_t> *vec, size_t index)
{
    if (index >= vec->size) { return VEC_ERROR; }
    
    if (index < vec->size - 1)
    {
        memmove(vec->data + index, vec->data + index + 1, (vec->size - index - 1) * sizeof(vec_elem_t));
    }

    vec->size--;
    vec->data[vec->size] = 0;
    
    return VEC_SUCCESS;
}


#endif