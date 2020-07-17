#include "vector.h"

vector_t *vector_init(size_t item_size)
{
    vector_t *new_vector = malloc(sizeof(vector_t)); 
    if (!new_vector) 
        return NULL;

    new_vector->item_size = item_size; 
    new_vector->size = 0; 
    new_vector->data = NULL;

    return new_vector;
}

void vector_destroy(vector_t *vector)
{
    if (vector->data) 
    {
        free(vector->data);
    }
    free(vector); 
}

void* vector_add(vector_t *vector, void *item)
{
    
    if (!vector->data)
    {
        vector->data = malloc(vector->item_size);
        if (!vector->data)
        {
            printf("Error on adding first item");
            return NULL;
        }
    }
    else
    {
        void *new_data = realloc(vector->data, vector->item_size * (vector->size+1));
        if (!new_data)
        {
            printf("Error on adding item");
            return NULL;            
        }
        vector->data = new_data;
    }

    vector->data[vector->size] = item;

    vector->size++;

    return vector->data[vector->size-1];
}

void *vector_get(vector_t *vector, size_t index)
{
    if (index >= vector->size)
    {
        return NULL;
    }
    
    return vector->data[index];
}

size_t vector_size(vector_t *vector)
{
    return vector->size;
}

int vector_remove(vector_t *vector, void *item)
{
    void *selected;
    for (size_t i = 0; i < vector->size; i++)
    {
        if (vector->data[i] == item)
        {
            selected = item;
            break;
        }
    }
    
    if (!selected)
        return -1;

    unsigned char *new_data = malloc(vector->item_size * (vector->size - 1));
    unsigned char *old_data = *vector->data;

    size_t j = 0; // j is the pointer in the current index of new_data array
    for (size_t i = 0; i < vector->size; i++)
    {
        if (vector->data[i] == selected)
            continue;
        memcpy(new_data + (j * vector->item_size), new_data + (i * vector->item_size), vector->item_size);
        j++;
    }

    free(old_data);
    *vector->data = new_data;
    vector->size--;

    return 0;
}

int vector_remove_from(vector_t *vector, size_t index)
{
    if (index >= vector->size)
        return -1;

    unsigned char *new_data = malloc(vector->item_size * (vector->size - 1));
    unsigned char *old_data = *vector->data;

    size_t j = 0; // j is the pointer in the current index of new_data array
    for (size_t i = 0; i < vector->size; i++)
    {
        if (i == index)
            continue;
        memcpy(new_data + (j * vector->item_size), new_data + (i * vector->item_size), vector->item_size);
        j++;
    }

    free(old_data);
    *vector->data = new_data;
    vector->size--;

    return 0;
}

void vector_empty(vector_t *vector)
{
    vector->size = 0;
    if (vector->data) 
    {
        vector->data = NULL; 
    }
}