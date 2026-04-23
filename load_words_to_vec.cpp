#include <sys/stat.h>
#include <ctype.h>
#include "vector.hpp"


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

    char *buffer = (char*)calloc((size_t)(file_size + 1), sizeof(char));
    if (buffer == NULL) { return NULL; }
    
    size_t actual_size = fread(buffer, 1, (size_t)file_size, stream);
    buffer[actual_size] = '\0';

    bool in_word = false;
    for (size_t i = 0; i < actual_size; i++)
    {
        if (isspace((unsigned char)buffer[i]) || buffer[i] == ',' || buffer[i] == '.' || buffer[i] == '!'
        || buffer[i] == '?' || buffer[i] == ':' || buffer[i] == '\"' || buffer[i] == '\'')
        {
            buffer[i] = '\0';
            in_word = false;
        }
        else if (!in_word)
        {
            vec_push_back(words, &buffer[i]);
            in_word = true;
        }
    }

    return buffer; 
}