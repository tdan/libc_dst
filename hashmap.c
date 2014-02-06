#include "hashmap.h"
#include <string.h> // for memcpy

static hash_node_t* _get_node(hashmap_t *map, void *key);

hashmap_t *create_hashmap(int buckets, unsigned key_size, unsigned value_size) {
	hashmap_t *map = (hashmap_t*)malloc(sizeof(hashmap_t));
	
	if(buckets < 1)
		map->num_buckets = NUMBER_OF_BUCKETS;
	else
		map->num_buckets = buckets;
	map->buckets = (list_t**)calloc(map->num_buckets, sizeof(list_t*));
	map->key_size = key_size;
	map->value_size = value_size;
		
	map->hash_func = &_default_hash;
	map->compare = &_default_compare;
	map->free_key = &free;
	map->free_value = &free;
	
	return map;
}

void hashmap_set_hash_func(hashmap_t *map, unsigned (*func)(void*)) {
	if(map == NULL)
		return;
	map->hash_func = func;
}

void hashmap_set_comp_func(hashmap_t *map, int (*func)(void*, void*)) {
	if(map == NULL)
		return;
	map->compare = func;
}

void hashmap_set_free_func(hashmap_t* map, void (*free_key)(void*), void (*free_value)(void*)) {
	map->free_key = free_key;
	map->free_value = free_value;
}

void hashmap_put(hashmap_t *map, void* key void* value) {
	if(map == NULL || key == NULL || value == NULL)
		return;
		
	int hash_val = map->hash_func(key, map->key_size);
	int bucket_num = hash_val % map->num_buckets;
	
	hash_node_t *node = _get_node(map, key);
	if(node != NULL) {
		memcpy(node->value, value, map->value_size);
		return;
	}
	
	node = (hash_node_t*)malloc(sizeof(hash_node_t));
	node->key = malloc(key_size);
	memcpy(node->key, key, map->key_size);
	node->value = malloc(value_size);
	memcpy(node->value, value, map->value_size);
	node->hash_val = hash_val;
	
	if(map->buckets[bucket_num] == 0) 
		map->buckets[bucket_num] = create_list(sizeof(hash_node_t*));
		
	list_push_front(map->buckets[bucket_num], node);
}

void* hashmap_get(hashmap_t* map, void *key) {
	hash_node_t *node = _get_node(map, key);
	return (node != NULL) ? node->data : NULL;
}

void hashmap_remove(hashmap_t *map, void *key) {
	if(map == NULL || key == NULL)
		return;
	
	int bucket_num = map->hash_func(key, map->key_size) % map->num_buckets;
	list_node_t* iter = list_iterator(map->buckets[bucket_num]);
	while(iter != NULL) {
		hash_node_t *node = (hash_node_t*) iter->data;
		if(map->compare(node->key, key, map->key_size) == 0) {
			map->free_key(node->key);
			map->free_value(node->value);
			list_delete(map->buckets[bucket_num], iter);
			return;
		}
		iter = iter->next;
	}
}

void destroy_hashmap(hashmap_t *map) {
	int i = 0;
	for(;i < map->num_buckets; i++) {
		list_node_t *iter = list_iterator(map->buckets[i]);
		while(iter != NULL) {
			hash_node_t* node = (hash_node_t*)iter->data;
			list_node_t *next = iter->next;
			map->free_key(node->key);
			map->free_value(node->value);
			free(node);
			iter = iter->next;
		}
		free(map->buckets[i]);
	}
	free(map);
}

static hash_node_t* _get_node(hashmap_t* map, void *key) {
	if(map == NULL || key == NULL)
		return NULL;

	int bucket_num = map->hash_func(key, map->key_size) % map->num_buckets;
	list_t *bucket = map->buckets[bucket_num];
	list_node_t *iter = list_iterator(bucket);
	while(iter != NULL) {
		hash_node_t *node = (hash_node_t*)iter->data;
		if(map->compare(node->key, key, map->key_size) == 0)
			return node;
		iter = iter->next;
	}

	return NULL;
}
