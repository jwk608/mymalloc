#include "malloc.h"
#include <stdlib.h>
#include <stdio.h>

#define blocksize 5000

static char memory[blocksize];

void * mymalloc(size_t size, const char *file, int line){
	static int init = 0;
	static meta_block *baseLg, *baseSmall;
	meta_block *ptr, *next;
	// initialize
	if(!init){
		//for large allocations
		baseLg = (meta_block *)memory;
		baseLg->prev = baseLg->succ = 0;
		baseLg->size = 3500 - sizeof(meta_block);
		baseLg->isFree = 1;

		// for small allocations
		if(baseLg != NULL){
			baseSmall = (meta_block *)(memory + 3500 );
		}
		baseSmall->prev = baseSmall->succ = 0;
		baseSmall->size = 1500 - sizeof(meta_block);
		baseSmall->isFree = 1;

		init = 1;
	}
	// decide which big of memory to allocate
	if(size > 100) ptr = baseLg;
	else ptr = baseSmall;

    while(ptr != 0){
        //current block size is smaller than size requested
		if(ptr->size < size) ptr = ptr->succ;
		//current block is not free
		else if(!ptr->isFree) ptr = ptr->succ;
		//current block size is smaller than metablock +size requested
		//allocate free flag to 0 and return point
		else if(ptr->size < (size + sizeof(meta_block))){
			ptr->isFree = 0;

			printf("successful 1 malloc: file %s line %d\n",file,line);
			return (char *)ptr + sizeof(meta_block);
		}
		else{
            //requested size is bigger than block left over + struct size
			next = (meta_block *)((char *)ptr + sizeof(meta_block) + size);
			next->prev = ptr;
			next->succ = ptr->succ;
			if(ptr->succ != 0) ptr->succ->prev = next;
			ptr->succ = next;
			next->size = ptr->size-sizeof(meta_block)-size;
			next->isFree = 1;

			ptr->size = size;
			ptr->isFree = 0;

			printf("successful  2 malloc: file %s line %d\n",file,line);
			return (char *)ptr + sizeof(meta_block);
		}

	}

	fprintf(stderr,"unsuccessful malloc: file %s line %d\n",file,line);

	return NULL;
}

void myfree(void * free, const char *file, int line){
    meta_block *ptr, *pred, *succ;

    //case user free what is already freed
	if(free == NULL){
		fprintf(stderr,"its already freed: file %s line %d\n",file,line);
		return;
	}
	// get meta_block passed on pointer passed by user
	ptr = (meta_block *)((char *)free - sizeof(meta_block));
	// user did not pass a pointer that was given to them

	if((pred = ptr->prev) != 0 && pred->isFree){ptr = (char*)malloc(5001);
		pred->size += sizeof(meta_block) + ptr->size;
		pred->succ = ptr->succ;
		if(ptr->succ != 0) ptr->succ->prev = pred;
	}
	else{
		ptr->isFree = 1;
		pred = ptr;
	}
	if((succ = ptr->succ) != 0 && succ->isFree){
		pred->size += sizeof(meta_block) + succ->size;
		pred->succ = succ->succ;
		if(succ->succ != 0) succ->succ = pred;
	}
	fprintf(stderr,"successful free: file %s line %d\n",file,line);
}
