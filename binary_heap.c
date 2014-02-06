#include "binary_heap.h"
#include <string.h>

void percUp(binary_heap_t *heap, unsigned pos);
void percDown(binary_heap_t *heap, unsigned pos);

binary_heap_t* create_heap(unsigned data_size) {
	binary_heap_t *heap = (binary_heap_t*)malloc(sizeof(binary_heap_t));
	heap->compare = &_default_compare;
	heap->vector = create_vector(data_size, 0);

	return heap;
}

void heap_set_free_func(binary_heap_t* heap, void (*func)(void*)) {
	vector_set_free_func(heap->vector, func);
}

void heap_set_comp_func(binary_heap_t *heap, 
			int (*func)(void*,void*,unsigned)) {
	heap->compare = func;
}

void heap_insert(binary_heap_t *heap, void *data) {
	if(heap == NULL || data == NULL)
		return;

	vector_add(heap->vector, data);
	// percolating up
	percUp(heap, heap->vector->size-1);
}

void* heap_front(binary_heap_t *heap) {
	return vector_get(heap->vector, 0);
}

void* heap_remove_front(binary_heap_t *heap) {
	if(heap == NULL)
		return NULL;

	// swap first and last elements
	vector_swap(heap->vector, 0, heap->vector->size-1);
	void *ret = vector_remove(heap->vector, heap->vector->size-1);
	// percolating down
	percDown(heap, 0);
	
	return ret;
}

unsigned heap_size(binary_heap_t *heap) {
	if(heap == NULL)
		return 0;
	return heap->vector->size;
}

void destroy_heap(binary_heap_t *heap) {
	destroy_vector(heap->vector);
	free(heap);
}

void percUp(binary_heap_t *heap, unsigned pos) {
	if(heap == NULL)
		return;
	
	vector_t *v = heap->vector;
	unsigned n = pos;
	while(n > 0 && 
			heap->compare(vector_get(v, n), 
						vector_get(v, (n-1)/2),
						heap->data_size) < 0) {
		vector_swap(v, n, (n-1)/2);
		n = (n-1)/2;
	}
}

void percDown(binary_heap_t *heap, unsigned pos) {
	if(heap == NULL)
		return;

	vector_t *v = heap->vector;
	unsigned minChild, n = pos;
	void *left, *right;
	while(2*n + 1 < v->size) {
		left = vector_get(v, 2*n + 1);
		right = vector_get(v, 2*(n+1));
		if(right == NULL ||
				heap->compare(left, right, heap->data_size) < 0) 
			minChild = 2*n + 1;
		else
			minChild = 2*(n+1);
		
		if(heap->compare(vector_get(v, n), 
						vector_get(v, minChild),
						heap->data_size) < 0)
			vector_swap(v, n, minChild);
		
		n = minChild;
	}
}
