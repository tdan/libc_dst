#ifndef _HASH_MAP_H_
#define _HASH_MAP_H_
#include <stdlib.h>
#include "list.h"
#include "utils.h"

#define NUMBER_OF_BUCKETS 200

typedef struct hash_node {
	void *key;
	void *value;
	unsigned hash_val;
} hash_node_t;

typedef struct hash_map {
	list_t **buckets;
	int num_buckets;
	unsigned key_size;
	unsigned value_size;
	unsigned (*hash_func)(void*, unsigned);
	int (*compare)(void*,void*,unsigned);
	void (*free_key)(void*);
	void (*free_value)(void*);
} hashmap_t;

hashmap_t* create_hashmap(int buckets, unsigned key_size, unsigned value_size);

void hashmap_set_hash_func(hashmap_t *map, unsigned (*func)(void*));

void hashmap_set_comp_func(hashmap_t *map, int (*func)(void*, void*)); 

void hashmap_set_free_func(hashmap_t *map, void (*free_key)(void*), void (*free_value)(void*));

void hashmap_put(hashmap_t *map, void *key, void *value);

void* hashmap_get(hashmap_t *map, void *key);

void hashmap_remove(hashmap_t *map, void *key);

void destroy_hashmap(hashmap_t *map);

#endif
