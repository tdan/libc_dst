#include "list.h"
#include <string.h>

list_t* create_list(size_t data_size) {
	list_t *list = (list_t *)malloc(sizeof(list_t));
	list->size = 0;
	list->front = NULL;
	list->back = NULL;
	list->data_size = data_size;
	// default free_data function is 
	// free
	list->free_data = &free;

	return list;
}

void list_set_free_func(list_t *list, void (*free_func)(void*)) {
	list->free_data = free_func;
}

void list_push_back(list_t *list, void *data) {
	list_insert(list, list->front, data);
}

void list_push_front(list_t *list, void *data) {
	list_insert(list, list->back, data);
}

void* list_pop_front(list_t *list) {
	return list_remove(list, list->front);
}

void* list_pop_back(list_t *list) {
	return list_remove(list, list->back);
}

void* list_front(list_t *list) {
	return (list != NULL || list->front != NULL) ? list->front->data : NULL; 
}

void* list_back(list_t *list) {
	return (list != NULL || list->back != NULL) ? list->back->data : NULL; 
}

void list_insert(list_t *list, list_node_t *pos, void *data) {
	if(list == NULL || data == NULL)
		return;
			
	list_node_t *new_node = (list_node_t*)malloc(sizeof(list_node_t));
	new_node->data = malloc(list->data_size);
	memcpy(new_node->data, data, list->data_size);

	if(list->size == 0) {
		new_node->next = new_node->prev = NULL;
		list->front = list->back = new_node;
	}
	else if(pos == list->front) {
		new_node->next = list->front;
		new_node->prev = NULL;
		list->front->prev = new_node;
		list->front = new_node;
	}
	else if(pos == list->back || pos == NULL) {
		new_node->next = NULL;
		new_node->prev = list->back;
		list->back->next = new_node;
		list->back = new_node;
	}
	else {
		new_node->next = pos;
		new_node->prev = pos->prev;
		pos->prev->next = new_node;
		pos->prev = new_node;
	}

	list->size++;
}

void* list_remove(list_t *list, list_node_t *node_removed) {
	if(list == NULL || node_removed == NULL)
		return NULL;
	
	void *ret = node_removed->data;
	
	if(list->size == 1) 
		list->front = list->back = NULL;
	else if(node_removed == list->front) {
		list->front = list->front->next;
		// new list->front
		list->front->prev = NULL;
	}
	else if(node_removed == list->back) {
		list->back = list->back->prev;
		list->back->next = NULL;
	}
	else {
		node_removed->next->prev = node_removed->prev;
		node_removed->prev->next = node_removed->next;
	}

	free(node_removed);
	list->size--;
	return ret;
}

void list_delete(list_t *list, list_node_t *node_deleted) {
	void *data = list_remove(list, node_deleted);
	list->free_data(data);
}

list_node_t* list_iterator(list_t *list) {
	return (list != NULL) ? list->front : NULL;
}

list_node_t* list_iterator_back(list_t *list) {
	return (list != NULL) ? list->back : NULL;
}

void destroy_list(list_t *list) {
	if(list == NULL)
		return;

	list_node_t *cur = list->front;
	list_node_t *next;
	/* free each element in the list */
	while(cur != NULL) {
		next = cur->next;
		list->free_data(cur->data);
		free(cur);
		cur = next;
	}
	/* free the whole list */
	free(list);
}
