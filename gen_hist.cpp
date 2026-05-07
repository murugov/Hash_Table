#include "hash_table.hpp"
#include <stdio.h>
#include <stdlib.h>

#define OUTPUT_PLOT_CMD "plot_hist.gp"

typedef hash_t (*hash_func_t)(ht_entry_t*);

ht_err_t analyze_all_hashes(ht_t<ht_entry_t*> *ht, vec_t<char*> *words);
ht_err_t rehash_ht(ht_t<ht_entry_t*> *ht, vec_t<char*> *words, ht_entry_t *buckets, hash_func_t hash_func);
ht_err_t gen_hist(ht_t<ht_entry_t*> *ht, const char *name);

hash_t const_hash(ht_entry_t *bucket);
hash_t first_char_hash(ht_entry_t *bucket);
hash_t len_hash(ht_entry_t *bucket);
hash_t checksum_hash(ht_entry_t *bucket);
hash_t roll_hash(ht_entry_t *bucket);
hash_t crc32_hash(ht_entry_t* bucket); 

struct hash_info
{
    hash_func_t func;
    const char* name;
};

hash_info functions[] = {
    {const_hash,      "hist_const"},
    {first_char_hash, "hist_first"},
    {len_hash,        "hist_len"},
    {checksum_hash,   "hist_checksum"},
    {roll_hash,       "hist_roll"},
    {crc32_hash,      "hist_crc32"}
};


ht_err_t analyze_all_hashes(ht_t<ht_entry_t*> *ht, vec_t<char*> *words)
{
    size_t words_size = words->size;
    const int number_hash_func = sizeof(functions) / sizeof(hash_info);

    for (int i = 0; i < number_hash_func; i++)
    {
        ht_entry_t *buckets = (ht_entry_t*)calloc_ex(words_size, sizeof(ht_entry_t));
        if (buckets == NULL) { return HT_ERROR; }

        rehash_ht(ht, words, buckets, functions[i].func);

        gen_hist(ht, functions[i].name);

        ht_clear(ht);
        free(buckets);
    }
    
    return HT_SUCCESS;
}


ht_err_t rehash_ht(ht_t<ht_entry_t*> *ht, vec_t<char*> *words, ht_entry_t *buckets, hash_func_t hash_func)
{
    size_t words_size = words->size;
    ht_entry_t *cur_bucket = NULL;

    for (size_t i = 0; i < words_size; i++)
    {
        char *word = words->data[i]; 

        cur_bucket = &buckets[i];

        cur_bucket->word = word;
        cur_bucket->hash = HASH_FUNC(cur_bucket);

        ht_insert<ht_entry_t*, EQUAL_FUNC>(ht, cur_bucket, hash_func);
    }

    return HT_SUCCESS;
}


ht_err_t gen_hist(ht_t<ht_entry_t*> *ht, const char *name)
{
    int *arr = (int*)calloc(HT_SIZE, sizeof(int));
    if (arr == NULL) { return HT_ERROR; }

    int min_idx = HT_SIZE;
    int max_idx = 0;
    bool has_data = false;

    for (int i = 0; i < HT_SIZE; i++) 
    {
        if (ht[i].vec != NULL && ht[i].vec->size > 0)
        {
            arr[i] = (int)(ht[i].vec)->size;
            if (i < min_idx) min_idx = i;
            if (i > max_idx) max_idx = i;
            has_data = true;
        }
        else
        {
            arr[i] = 0;
        }
    }

    if (!has_data)
    {
        min_idx = 0;
        max_idx = HT_SIZE;
    }
    else
    {
        min_idx = (min_idx > 10) ? min_idx - 10 : 0;
        max_idx = (max_idx < HT_SIZE - 10) ? max_idx + 10 : HT_SIZE;
    }

    FILE *data_file = fopen("hist_data.tmp", "w");
    if (data_file == NULL) 
    { 
        free(arr);
        return HT_ERROR; 
    }
    for (int i = 0; i < HT_SIZE; i++)
    {
        fprintf(data_file, "%d %d\n", i, arr[i]);
    }
    fclose(data_file);

    FILE *gp_script = fopen(OUTPUT_PLOT_CMD, "w");
    if (gp_script == NULL) 
    { 
        free(arr);
        return HT_ERROR; 
    }

    fprintf(gp_script, "set terminal pngcairo size 1200,600 enhanced font 'Arial,12'\n");
    fprintf(gp_script, "set output 'reports/histograms/%s.png'\n", name);
    fprintf(gp_script, "set style fill solid 0.7\n");
    fprintf(gp_script, "set boxwidth 0.8\n");
    fprintf(gp_script, "set xlabel 'Номер ячейки'\n");
    fprintf(gp_script, "set ylabel 'Количество коллизий (размер бакета)'\n");
    fprintf(gp_script, "set title 'Распределение хеш-функции: %s'\n", name);
    fprintf(gp_script, "set grid ytics\n");
    
    fprintf(gp_script, "set yrange [0:*]\n");
    fprintf(gp_script, "set xrange [%d:%d]\n", min_idx, max_idx);
    
    fprintf(gp_script, "plot 'hist_data.tmp' using 1:2 with boxes lc rgb '#228B22' notitle\n");
    fclose(gp_script);

    char command[128];
    snprintf(command, sizeof(command), "gnuplot %s", OUTPUT_PLOT_CMD);
    int status = system(command);
    if (status != 0) {
        fprintf(stderr, "Warning: Gnuplot failed for %s\n", name);
    }

    remove("hist_data.tmp");
    remove(OUTPUT_PLOT_CMD);
    free(arr);

    return HT_SUCCESS;
}
