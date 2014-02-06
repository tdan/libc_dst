#ifndef _RB_TREE_H_
#define _RB_TREE_H_
#include <stdlib.h>
#include "utils.h"

typedef enum { IN, PRE, POST } ORDER;
typedef enum { RED, BLACK } COLOR;

typedef struct tree_node {
	void *data;
	struct tree_node *parent;
	struct tree_node *left;
	struct tree_node *right;
	COLOR color;
} tree_node_t;

typedef struct rbtree {
	tree_node_t *root;
	unsigned data_size;
	unsigned size;
	int (*compare)(void*,void*,unsigned);
	void (*free_func)(void*);
} rbtree_t;

rbtree_t* create_tree(unsigned data_size, int (*comp)(void*, void*, unsigned));

void tree_set_comp_func(rbtree_t *tree, int (*comp)(void*, void*, unsigned));

void tree_set_free_func(rbtree_t *tree, void (*free_func)(void*));

tree_node_t* tree_insert(rbtree_t *tree, void *data);

void* tree_remove(rbtree_t *tree, tree_node_t *node);

tree_node_t* tree_search(rbtree_t *tree, void *data);

void tree_traverse(rbtree_t *tree, ORDER order, void (*op)(void*));

void* tree_min(rbtree_t *tree);

void* tree_max(rbtree_t *tree);

tree_node_t* _grandparent(tree_node_t *node);
tree_node_t* _sibling(tree_node_t *node);
tree_node_t* _uncle(tree_node_t *node);
tree_node_t* _pred(tree_node_t *node);
tree_node_t* _succ(tree_node_t *node);

void destroy_tree(rbtree_t *tree);

#endif
