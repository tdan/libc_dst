#include "utils.h"

int* INT(const int const_int) {
	int *int_ptr = (int*)malloc(sizeof(int));
	memcpy(int_ptr,&const_int, sizeof(int));
	return int_ptr;
}

char* STR(const char* const_str) {
	char *str = (char*)calloc(strlen(const_str) + 1,sizeof(char));
	return strcpy(str, const_str);
}

int _default_compare(void *k1, void *k2, unsigned key_size) {
	unsigned char* k1_ptr = (char*)k1;
	unsigned char* k2_ptr = (char*)k2;

	int i;
	for(i = 0; i < key_size; i++)
		if(*(k1_ptr + i) < *(k2_ptr + i))
			return -1;
		else if(*(k1_ptr + i) > *(k2_ptr + i))
			return 1;
	
	return 0;
}

unsigned _default_hash(void *key, unsigned key_size) {
	unsigned char *key_ptr = (char*)key;
	unsigned hash = 0;
	
	int c, i;
	for(i = 0; i < key_size; i++) {
		c = *(key_ptr + i);
		hash = c + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}

