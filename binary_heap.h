#ifndef _BINARY_HEAP_H_
#define _BINARY_HEAP_H_
#include <stdlib.h>
#include "vector.h"
#include "utils.h"

typedef struct binary_heap {
	vector_t *vector;
	unsigned data_size;
	int (*compare)(void*, void*, unsigned);
} binary_heap_t;

binary_heap_t* create_heap(unsigned data_size, int (*comp_func)(void*,void*));

void heap_set_free_func(binary_heap_t *heap, void (*func)(void*));

void heap_insert(binary_heap_t* heap, void *data);

void* heap_front(binary_heap_t* heap);

void* heap_remove_front(binary_heap_t* heap);

unsigned heap_size(binary_heap_t *heap);

void destroy_heap(binary_heap_t* heap);

#endif
