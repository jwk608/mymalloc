#ifndef MALLOC_H
#define MALLOC_H

/*
 * malloc.h
 */

#include <stdlib.h>
#include<stddef.h>

typedef struct meta_block{
struct meta_block *prev, *succ;
	int isFree;
    size_t size;
	int recognize;
	const char *file;
	int line;
} meta_block;

void * mymalloc(size_t size, const char *file, int line);

void myfree(void * p1, const char *file, int line);

#endif
