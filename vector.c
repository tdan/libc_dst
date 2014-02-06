#include "vector.h"
#include <string.h> // for memcpy

vector_t* create_vector(unsigned data_size, unsigned capacity) {
	vector_t *v = (vector_t*)malloc(sizeof(vector_t));
	
	if(capacity < 1)
		capacity = INIT_CAP;
	v->capacity = capacity;
	
	v->data = (void**)calloc(capacity, sizeof(void*));

	v->size = 0;
	v->data_size = data_size;
	v->free_data = &free;

	return v;
}

void vector_set_free_func(vector_t *v, void (*func)(void*)) {
	v->free_data = func;
}

void vector_add(vector_t *v, void *data) {
	if(v == NULL || data == NULL)
		return;

	// increase the capacity of vector if size approaches capacity 
	if(v->size == v->capacity - 1) {
		v->data = (void**)realloc(v->data, sizeof(void*) * v->capacity * 4);
		v->capacity *= 4;
	}

	(v->data)[v->size] = (void*)malloc(v->data_size);
	memcpy((v->data)[v->size], data, v->data_size);
	v->size++;
}

void vector_set(vector_t *v, unsigned position, void *data) {
	if(v == NULL || position > v->size - 1)
		return;
	
	if(data == NULL)
		vector_remove(v, position);
	else 
		memcpy(v->data[position], data, v->data_size);
}

void* vector_remove(vector_t *v, unsigned position) {
	if(v == NULL || position > v->size - 1)
		return;

	if(v->size < v->capacity / 4) {
		v->data = (void**)realloc(v->data, sizeof(void*) * v->capacity / 2);
		v->capacity /= 2;
	}

	void *ret = v->data[position];
	
	int i;
	for(i = position; i < v->size - 1; i++) 
		v->data[i] = v->data[i+1];
	v->data[v->size] = NULL;
	v->size--;

	return ret;
}

void* vector_get(vector_t *v, unsigned position) {
	if(v == NULL || position > v->size - 1)
		return NULL;
	return v->data[position];
}

void vector_swap(vector_t *v, unsigned i, unsigned j) {
	if(v == NULL)
		return;
	void *tmp = v->data[i];
	v->data[i] = v->data[j];
	v->data[j] = tmp;
}

int vector_indexOf(vector_t *v, void *data) {
	if(v != NULL) {
		int i;
		for(i = 0; i < v->size; i++)
			if(v->data[i] == data)
				return i;
	}
	return -1;
}

void destroy_vector(vector_t *v) {
	if(v == NULL)
		return;

	int i;
	for(i = 0; i < v->size; i++)
		v->free_data(v->data[i]);
	free(v->data);
	free(v);
}
