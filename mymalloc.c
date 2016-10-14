#include "malloc.h"
#include <stdlib.h>
#include <stdio.h>

#define blocksize 5000

static char memory[blocksize];


void freecheck(){
	meta_block *ptr = (meta_block *)memory;
	while(ptr != 0){
		if(ptr->recognize != 0xFFFFFFFF){
			printf("did not free memory: file %s line %d\n", ptr->file,ptr->line);
		}
		ptr = ptr->succ;
	}

	ptr = (meta_block *)(memory + 3000);
	while(ptr != 0){
		if(ptr->recognize != 0xFFFFFFFF){
			printf("did not free memory: file %s line %d\n",ptr->file,ptr->line);
		}
		ptr = ptr->succ;
	}
}

void * mymalloc(size_t size, const char *file, int line){
	static int initialized = 0;
	static meta_block *rootBig, *rootSmall;
	meta_block *ptr, *next;
	// initialize one time
	if(!initialized){
		atexit(freecheck);
		// root for large allocations
		rootBig = (meta_block *)memory;
		rootBig->prev = rootBig->succ = 0;
		rootBig->size = 3000 - sizeof(meta_block);
		rootBig->isFree = 1;
		rootBig->recognize = 0xFFFFFFFF;
		// root for small alloca    tions
		if(rootBig != NULL){
			rootSmall = (meta_block *)(memory + 3000 );
		}
		rootSmall->prev = rootSmall->succ = 0;
		rootSmall->size = 2000 - sizeof(meta_block);
		rootSmall->isFree = 1;
		rootSmall->recognize = 0xFFFFFFFF;
		initialized = 1;
	}
	// decide which chunk of memory to allocate
	if(size > 50) ptr = rootBig;
	else ptr = rootSmall;

    while(ptr != 0){
        //current block size is smaller than size requested
		if(ptr->size < size) ptr = ptr->succ;
		//current block is not free
		else if(!ptr->isFree) ptr = ptr->succ;
		//current block size is smaller than metablock +size requested
		//allocate free flag to 0 and return point
		else if(ptr->size < (size + sizeof(meta_block))){
			ptr->isFree = 0;
			ptr->recognize = 0xAAAAAAAA;
			ptr->file = file;
			ptr->line = line;
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
			next->recognize = 0xFFFFFFFF;
			ptr->size = size;
			ptr->isFree = 0;
			ptr->recognize = 0xAAAAAAAA;
			ptr->file = file;
			ptr->line = line;
			printf("successful  2 malloc: file %s line %d\n",file,line);
			return (char *)ptr + sizeof(meta_block);
		}

	}

	printf("unsuccessful malloc: file %s line %d\n",file,line);

	return NULL;
}

void myfree(void * p1, const char *file, int line){
	if(p1 == NULL){
		printf("invalid free: file %s line %d\n",file,line);
		return;
	}
	meta_block *ptr, *pred, *succ;
	// get meta_block passed on pointer passed by user
	ptr = (meta_block *)((char *)p1 - sizeof(meta_block));
	// user did not pass a pointer that was given to them
	if(ptr->recognize != 0xAAAAAAAA){
		printf("invalid free: file %s line %d\n",file,line);
		return;
	}
	// change recognition pattern so user does not free this pointer again
	else{
		ptr->recognize = 0xFFFFFFFF;
	}

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
	printf("successful free: file %s line %d\n",file,line);
}
