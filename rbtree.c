#include "rbtree.h"
#include <string.h>

static void _place_node(rbtree_t* tree, tree_node_t *node);
static void _right_rotate(rbtree_t* tree, tree_node_t *node);
static void _left_rotate(rbtree_t* tree, tree_node_t *node);

static tree_node_t* _findMax(tree_node_t* root);
static tree_node_t* _findMin(tree_node_t* root);

static void _fix_insert(rbtree_t *tree, tree_node_t *node);
static void _fix_delete(rbtree_t *tree, tree_node_t *node);

rbtree_t* create_tree(unsigned data_size,
					int (*comp)(void*,void*,unsigned)) {
	rbtree_t *tree = (rbtree_t*)malloc(sizeof(rbtree_t));

	tree->root = NULL;
	tree->data_size = data_size;
	tree->size = 0;

	if(comp == NULL)
		tree->compare = &_default_compare;
	else
		tree->compare = comp;
	tree->free_func = &free;

	return tree;
}

void tree_set_comp_func(rbtree_t *tree, 
						int (*comp)(void*, void*, unsigned)) {
	if(tree == NULL)
		return;
	tree->compare = comp;
}

void tree_set_free_func(rbtree_t *tree, 
						void (*free_func)(void*)) {
	if(tree == NULL)
		return;
	tree->free_func = free_func;
}

tree_node_t* tree_insert(rbtree_t *tree, void *data) {
	if(tree == NULL || data == NULL)
		return NULL;

	tree_node_t *node = (tree_node_t*)malloc(sizeof(tree_node_t));
	node->data = malloc(tree->data_size);
	memcpy(node->data, data, tree->data_size);
	node->left = node->right = node->parent = NULL;
	node->color = RED;

	// place node in correct place
	if(tree->size == 0) {
		tree->root = node;
		node->color = BLACK;
		return;
	}

	_place_node(tree, node);
	_fix_insert(tree, node);

	tree->size++;

	return node;
}

void* tree_remove(rbtree_t *tree, tree_node_t *node) {
	if(tree == NULL || node == NULL)
		return NULL;

	void *ret = node->data;
	
	tree_node_t *cur, *next;
	if(node->left == NULL || node->right == NULL)
		cur = node;
	else
		cur = _succ(node);

	if(cur->left != NULL)
		next = cur->left;
	else
		next = cur->right;

	if(next != NULL)
		next->parent = cur->parent;
	
	if(cur->parent = NULL)
		tree->root = next;
	else if(cur == cur->parent->left)
		cur->parent->left = next;
	else 
		cur->parent->right = next;

	if(node != cur)
		node->data = cur->data;

	if(cur->color == BLACK)
		_fix_delete(tree, cur);

	free(cur);
	tree->size--;

	return ret;
}

tree_node_t *tree_search(rbtree_t *tree, void *data) {
	if(tree == NULL || data == NULL)
		return NULL;

	tree_node_t *cur = tree->root;
	while(cur != NULL) {
		if(tree->compare(data, 
					cur->data, tree->data_size) > 0)
			cur = cur->right;
		else if(tree->compare(data, 
					cur->data, tree->data_size) < 0)
			cur = cur->left;
		else
			return cur;
	}

	return NULL;
}

void* tree_max(rbtree_t *tree) {
	tree_node_t *max = _findMax(tree->root);
	return max != NULL ? max->data : NULL;
}

void* tree_min(rbtree_t *tree) {
	tree_node_t *min = _findMin(tree->root);
	return min != NULL ? min->data : NULL;
}

void destroy_tree(rbtree_t *tree){
	if(tree == NULL)
		return;

	while(tree->root != NULL) {
		void *data = tree_remove(tree, tree->root);
		tree->free_func(data);
	}
}

static void _fix_insert(rbtree_t *tree, tree_node_t *node) {
	tree_node_t *cur = node;
	while(cur != tree->root && 
			cur->parent->color == RED) {
		
		if(_uncle(cur)->color == RED) {
			cur->parent->color = BLACK;
			_grandparent(cur)->color = RED;
			_uncle(node)->color = BLACK;
			cur = _grandparent(cur);
		}
		else if(cur == cur->parent->right) {
			cur = cur->parent;
			_left_rotate(tree, cur);
			cur->parent->color = BLACK;
			_grandparent(cur)->color = RED;
			_right_rotate(tree, _grandparent(cur));
		}
		else {
			cur = cur->parent;
			_right_rotate(tree, cur);
			cur->parent->color = BLACK;
			_grandparent(cur)->color = RED;
			_left_rotate(tree, _grandparent(cur));
		}
	}

	tree->root->color = BLACK;
}

static void _fix_delete(rbtree_t *tree, tree_node_t *node) {
	tree_node_t *cur = node;
	tree_node_t *sibling;

	while(cur != tree->root &&
			cur->color == BLACK) {

		sibling = _sibling(cur);
		if(sibling->color == RED) {
			sibling->color = BLACK;
			cur->parent->color = RED;

			if(sibling == cur->parent->right) {
				_left_rotate(tree, cur->parent);
				sibling = cur->parent->right;
			}
			else {
				_right_rotate(tree, cur->parent);
				sibling = cur->parent->left;
			}
		}

		if(sibling->left->color == BLACK &&
				sibling->right->color == BLACK) {
			sibling->color = RED;
			cur = cur->parent;
		}
		else if(sibling == cur->parent->right &&
					sibling->right->color == BLACK) {
			sibling->left->color = BLACK;
			sibling->color = RED;
			_right_rotate(tree, sibling);
			sibling = cur->parent->right;
		}
		else if(sibling == cur->parent->left &&
					sibling->left->color == BLACK) {
			sibling->right->color = BLACK;
			sibling->color = RED;
			_left_rotate(tree, sibling);
			sibling = cur->parent->left;
		}
		else {
			sibling->color = cur->parent->color;
			cur->parent->color = BLACK;
			
			if(sibling == cur->parent->right) {
				sibling->right->color = BLACK;
				_left_rotate(tree, cur->parent);
			}
			else {
				sibling->left->color = BLACK;
				_right_rotate(tree, cur->parent);
			}
			
			cur = tree->root;
		}
	}

	cur->color = BLACK;
}


tree_node_t* _grandparent(tree_node_t* node) {
	if(node != NULL && node->parent != NULL)
		return node->parent->parent;
	return NULL;
}

tree_node_t* _uncle(tree_node_t* node) {
	if(node == NULL)
		return NULL;
	
	tree_node_t* grandparent = _grandparent(node);
	if(node->parent == grandparent->right)
		return grandparent->left;
	else
		return grandparent->right;
}

tree_node_t* _sibling(tree_node_t* node) {
	if(node == NULL || node->parent == NULL)
		return NULL;

	if(node == node->parent->left)
		return node->parent->right;
	else 
		return node->parent->left;
}

tree_node_t* _pred(tree_node_t *node) {
	if(node == NULL)
		return NULL;

	if(node->left != NULL)
		return _findMax(node->left);

	tree_node_t *pred = node->parent;
	while(pred != NULL && node != pred->left) {
		node = pred;
		pred = pred->parent;
	}

	return pred;
}

tree_node_t* _succ(tree_node_t *node) {
	if(node == NULL)
		return NULL;

	if(node->right != NULL)
		return _findMin(node->right);

	tree_node_t *succ = node->parent;
	while(succ != NULL && node != succ->right) {
		node = succ;
		succ = succ->parent;
	}

	return succ;
}

static void _place_node(rbtree_t *tree, tree_node_t *node) {
	tree_node_t *cur = tree->root;
	tree_node_t *parent = NULL;
	while(cur != NULL) {
		parent = cur;
		if(tree->compare(cur->data, 
						node->data, tree->data_size) < 0)
			cur = cur->left;
		else 
			cur = cur->right;
	}

	node->parent = parent;
	if(parent == NULL) return;

	if(tree->compare(parent->data, 
					node->data, 
					tree->data_size) > 0)
		parent->left = node;
	else
		parent->right = node;
}

static void _right_rotate(rbtree_t *tree, tree_node_t *node) {
	tree_node_t *left_node = node->left;
	if(left_node == NULL) return;
	
	if(left_node->right != NULL) {
		left_node->right->parent = node;
		node->left = left_node->right;
	}
	else
		node->left = NULL;

	left_node->parent = node->parent;

	if(node->parent == NULL) 
		tree->root = left_node;
	else if(node->parent->right == node)
		node->parent->right = left_node;
	else
		node->parent->left = left_node;

	left_node->right = node;
	node->parent = left_node;
}

static void _left_rotate(rbtree_t* tree, tree_node_t *node) {
	tree_node_t *right_node = node->right;
	if(right_node == NULL) return;

	if(right_node->left != NULL) {
		right_node->left->parent = node;
		node->right = right_node->left;
	}
	else
		node->right = NULL;

	right_node->parent = node->parent;

	if(node->parent == NULL)
		tree->root = right_node;
	else if(node->parent->left == node)
		node->parent->left = right_node;
	else
		node->parent->right = right_node;
	
	right_node->left = node;
	node->parent = right_node;
}

static tree_node_t* _findMax(tree_node_t *root) {
	if(root == NULL)
		return NULL;

	tree_node_t *cur = root;
	while(cur->right != NULL)
		cur = cur->right;

	return cur;
}

static tree_node_t* _findMin(tree_node_t *root) {
	if(root == NULL) 
		return NULL;

	tree_node_t *cur = root;
	while(cur->left != NULL)
		cur = cur->left;

	return cur;
}
