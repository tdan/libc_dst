#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>
#include "utils.h"

typedef struct list_node {
	void *data;
	struct list_node *next;
	struct list_node *prev;
} list_node_t;

typedef struct list {
	size_t size;
	size_t data_size;
	list_node_t *front;
	list_node_t *back;
	void (*free_data)(void*);
} list_t;

list_t* create_list(size_t data_size);

void list_set_free_func(list_t *list, void (*func)(void*));

void list_push_back(list_t *list, void *data);

void list_push_front(list_t *list, void *data);

void* list_pop_back(list_t *list);

void* list_pop_front(list_t *list);

void* list_front(list_t *list);

void* list_back(list_t *list);

void list_insert(list_t *list, list_node_t *node_before, void *data);

void list_delete(list_t *list, list_node_t *node_deleted);

void* list_remove(list_t *list, list_node_t *node_removed);

int list_contains(list_t *list, list_node_t *node);

list_node_t* list_iterator(list_t *list);

list_node_t* list_iterator_back(list_t *list);

void destroy_list(list_t* list);

#endif
