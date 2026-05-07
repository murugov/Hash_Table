#include <sys/stat.h>
#include <ctype.h>
#include "vector.hpp"


ssize_t find_file_size(FILE* stream);
char* load_words_to_vec(FILE *stream, vec_t<char*> *words);


ssize_t find_file_size(FILE* stream)
{
    struct stat file_info = {};
    if (fstat(fileno(stream), &file_info) != 0) { return -1; }

    return (ssize_t)file_info.st_size;
}


char* load_words_to_vec(FILE *stream, vec_t<char*> *words)
{
    if (stream == NULL) { return NULL; }

    ssize_t file_size = find_file_size(stream);
    if (file_size <= 0) { return NULL; }

    char *buffer = (char*)calloc_ex((size_t)(file_size + 1), sizeof(char));
    if (buffer == NULL) { return NULL; }
    
    size_t actual_size = fread(buffer, sizeof(char), (size_t)file_size, stream);
    buffer[actual_size] = '\0';

    bool in_word = false;
    size_t cur_len = 0;
    char *word_start = NULL;
    
    for (size_t i = 0; i < actual_size; i++)
    {
        if (isspace((unsigned char)buffer[i]) || buffer[i] == ',' || buffer[i] == '.' || buffer[i] == '!'
        || buffer[i] == '?' || buffer[i] == ':' || buffer[i] == ';' || buffer[i] == '\"' || buffer[i] == '\'')
        {
            if (in_word && cur_len < 32)
            {
                vec_push_back<char*>(words, word_start);
            }
            buffer[i] = '\0';
            in_word = false;
            cur_len = 0;
        }
        else if (!in_word)
        {
            word_start = &buffer[i];
            in_word = true;
            cur_len = 1;
        }
        else
        {
            cur_len++;
        }
    }

    size_t number_words = words->size;
    char *aligned_buffer = (char*)calloc_ex(number_words, 32);
    if (aligned_buffer == NULL) { return NULL; }

    for (size_t i = 0, j = 0; i < number_words; i++, j += 32)
    {
        strcpy(&aligned_buffer[j], words->data[i]);
        words->data[i] = &aligned_buffer[j];
    }
    
    free(buffer);

    return aligned_buffer; 
}