#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct vector
{
    size_t item_size;
    size_t size;
    void **data;
} vector_t;

vector_t *vector_init(size_t item_size);

void* vector_append(vector_t *vector);

void* vector_add(vector_t *vector, void *item);

void* vector_get(vector_t *vector, size_t index);

size_t vector_size(vector_t *vector);

int vector_remove(vector_t *vector, void *item);

int vector_remove_from(vector_t *vector, size_t index);

void vector_empty(vector_t *vector);

void vector_destroy(vector_t *vector);