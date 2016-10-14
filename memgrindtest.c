#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include "mymalloc.h"


void A(void* p, int i){
	if(i == 3000){
		return;
	}
	else{
		p = malloc(1);
		i++;
		void* q = NULL;
		A(q,i);
		free(p);
	}
}


void B(void* p){
	int i;
	for(i = 0; i < 3000; i++){
		p = malloc(1);
		free(p);
	}
}

int main(){

	int i;
	void* p = NULL;

	unsigned int timeA = 0;
	unsigned int timeB = 0;

	struct timeval start;
	struct timeval end;


	for(i = 0; i < 100; i++){

		gettimeofday(&start, NULL);
		A(p, 1);
		gettimeofday(&end, NULL);
		timeA += (end.tv_sec-start.tv_sec)*1000000 + end.tv_usec-start.tv_usec;
		

		gettimeofday(&start, NULL);
		B(p);
		gettimeofday(&end, NULL);
		timeB += (end.tv_sec-start.tv_sec)*1000000 + end.tv_usec-start.tv_usec;

	}

	timeA /= 100;
	timeB /= 100;


	printf("Average time (in microseconds) for workload A: %d\n", timeA);
	printf("Average time (in microseconds) for workload B: %d\n", timeB);

	return 0;
}
