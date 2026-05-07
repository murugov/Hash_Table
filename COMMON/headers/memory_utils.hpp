#ifndef MEMORY_UTILS_HPP
#define MEMORY_UTILS_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define ALIGNMENT 32ULL

inline void* malloc_ex(size_t __size);
inline void* calloc_ex(size_t __nmemb, size_t __size);
inline void* realloc_ex(void *__ptr, size_t __size);


#if defined(ALIGNED)

    __attribute__ ((always_inline))
    inline void* malloc_ex(size_t __size)
    {
        return aligned_alloc(ALIGNMENT, __size);
    }

    __attribute__ ((always_inline))
    inline void* calloc_ex(size_t __nmemb, size_t __size)
    {
        size_t total_size = __nmemb * __size;
        size_t padded_size = (total_size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);

        void *ptr = aligned_alloc(ALIGNMENT, padded_size);
        if (ptr) { memset(ptr, 0, __nmemb * __size); }
        return ptr;
    }

    __attribute__ ((always_inline))
    inline void* realloc_ex(void* __ptr, size_t __size)
    {
        if (!__ptr) { return malloc_ex(__size); }
        if (__size == 0) { free(__ptr); return NULL; }
        
        size_t old_size = malloc_usable_size(__ptr);
        
        if (old_size >= __size) { return __ptr; }
        
        void* new_ptr = malloc_ex(__size);
        if (!new_ptr) { return NULL; }
        
        memcpy(new_ptr, __ptr, old_size);
        free(__ptr);
        
        return new_ptr;
    }

#else

    __attribute__ ((always_inline))
    inline void* malloc_ex(size_t __size)
    {
        return malloc(__size);
    }

    __attribute__ ((always_inline))
    inline void* calloc_ex(size_t __nmemb, size_t __size)
    {
        return calloc(__nmemb, __size);
    }

    __attribute__ ((always_inline))
    inline void* realloc_ex(void *__ptr, size_t __size)
    {
        return realloc(__ptr, __size);
    }

#endif

#endif