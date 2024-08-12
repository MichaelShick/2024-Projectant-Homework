#include "hash_table.h"


hash_table_t* init_hash_table(size_t size) {
    hash_table_t* hash_table = (hash_table_t *)malloc(sizeof(hash_table_t));
    hash_table->size = HASH_TABLE_ARRAY_SIZE;
    hash_table->buckets_count = 0;
    hash_table->bytes_size = size;
    memset(hash_table->array, 0, sizeof(bucket_t*) * HASH_TABLE_ARRAY_SIZE); // Initialize the array to NULL

    return hash_table;
}

int hash(char *string) {
    if (!string)
        return -1;

    int first_char = string[0];
    int last_char = string[strlen(string) - 1];

    int index = (last_char * first_char + strlen(string)) % HASH_TABLE_ARRAY_SIZE;
    return index;
}

void* get(hash_table_t *hash_table, char *key) {
    if (hash_table == NULL || key == NULL) {
        return NULL;
    }

    int index = hash(key);
    bucket_t *irrat = hash_table->array[index];

    while (irrat != NULL) {
        if (strncmp(irrat->key, key, HASH_TABLE_MAX_STRING_SIZE) == 0) {
            return irrat->value;
        }
        irrat = irrat->next;
    }

    return NULL;
}

void insert(hash_table_t *hash_table, char *key, void *value) {
    if (hash_table == NULL || key == NULL || value == NULL) {
        perror("Invalid argument");
        return;
    }

    int index = hash(key);
    if (index == -1) {
        perror("Invalid key");
        return;
    }

    bucket_t *new_bucket = (bucket_t *)malloc(sizeof(bucket_t));
    if (new_bucket == NULL) {
        perror("Memory allocation failed");
        return;
    }

    strncpy(new_bucket->key, key, HASH_TABLE_MAX_STRING_SIZE);
    new_bucket->value = malloc(hash_table->bytes_size);
    if (new_bucket->value == NULL) {
        perror("Memory allocation for value failed");
        free(new_bucket);
        return;
    }
    memcpy(new_bucket->value, value, hash_table->bytes_size);
    new_bucket->next = hash_table->array[index];
    hash_table->array[index] = new_bucket;

    hash_table->buckets_count++;
}