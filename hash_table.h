#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "hash_table.h"

#define HASH_TABLE_ARRAY_SIZE 100
#define HASH_TABLE_MAX_STRING_SIZE 100

typedef struct bucket {
    char key[HASH_TABLE_MAX_STRING_SIZE];
    void *value;
    struct bucket *next;
} bucket_t;

typedef struct {
    size_t size;
    size_t bytes_size;
    size_t buckets_count;
    bucket_t *array[HASH_TABLE_ARRAY_SIZE];
} hash_table_t;

hash_table_t* init_hash_table(size_t size) ;
int hash(char *string) ;
void* get(hash_table_t *hash_table, char *key) ;
void insert(hash_table_t *hash_table, char *key, void *value) ;
// void free_hash_table(hash_table_t *hash_table) ;
// void free_bucket(bucket_t *bucket) ;
// void free_buckets(bucket_t *bucket) ;
