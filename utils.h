#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>
#include <string.h>

int* INT(const int const_int);

char* STR(const char* const_str);

int _default_compare(void *k1, void *k2, unsigned key_size);

unsigned _default_hash(void *key, unsigned key_size);

#endif
