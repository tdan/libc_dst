#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdlib.h>
#include "utils.h"

#define INIT_CAP 10

typedef struct vector {
	/* array of data */
	void **data;
	unsigned capacity;
	unsigned size;
	unsigned data_size;
	void (*free_data)(void*);
} vector_t;

vector_t* create_vector(unsigned data_size, unsigned capacity);

void vector_set_free_func(vector_t *v, void (*func)(void*));

void vector_add(vector_t *v, void *data);

void vector_set(vector_t *v, unsigned position, void *data);

void* vector_remove(vector_t *v, unsigned position);

void* vector_get(vector_t *v, unsigned position);

void vector_swap(vector_t *v, unsigned i, unsigned j);

int vector_indexOf(vector_t *v, void *data);

void destroy_vector(vector_t *v);

#endif
