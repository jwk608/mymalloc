#include <stdlib.h>
#include "mymalloc.h"
#include <time.h>
#include <sys/time.h>


void test_A() {
	char* ptr[3000];
	int i = 0;

	
	while (i < 3000) {
		ptr[i] = malloc(1);
		i++;
	}

	i = 0;


	while (i < 3000) {
		free(ptr[i]);
		ptr[i] = NULL;
		i++;
	}
}


void test_B() {
	char *ptr = NULL;
	int i = 0;


	while (i < 3000) {
		ptr = malloc(1);
		free(ptr);
		ptr = NULL;
		i++;
	}
}


void test_C() {
	char* ptr[3000]; //0-malloc 1-free
	int choice = rand() % 2,nextIndexToMalloc = 0,nextIndexToFree = 0;
	srand((unsigned int)time(NULL));

	//When the next index for malloc and free have both reached 3000, then we've performed 3000 of both operations. Terminate the loop.
	while ((nextIndexToMalloc + nextIndexToFree) < 6000) {
		
		//If (malloc) and (haven't malloc'd 3000 things yet
		if (choice == 0 && nextIndexToMalloc < 3000) {
			ptr[nextIndexToMalloc] = malloc(1);
			
			//If null was returned, don't advance to the next index.
			if (ptr[nextIndexToMalloc] != NULL) {
				nextIndexToMalloc++;
			}

		} else {
			
			//If the number of successful frees is less than the number of successful mallocs,
			if (nextIndexToFree < nextIndexToMalloc) {	
				free(ptr[nextIndexToFree]);
				ptr[nextIndexToFree] = NULL;
				nextIndexToFree++;
			}
		}

		//Make a new choice.
		choice = rand() % 2;
	}
}



float calculateMean(int arr[], int size) {
	int i = 0;
	int total = 0;	

	while (i < size) {
		total += arr[i];
		i++;
	}

	return (1.0*total/size);
}

int main() {
	int i = 0;
	int testATimes[100],testBTimes[100],testCTimes[100];

	struct timeval start ,end;

	// test A 100 times.
	while (i < 100) {
		gettimeofday(&start, NULL);
		test_A();
		gettimeofday(&end, NULL);
		testATimes[i] = end.tv_sec - start.tv_sec;
		i++;
	} 


	i = 0;

	//test B 100 times.
	while (i < 100) {
		gettimeofday(&start, NULL);
		test_B();
		gettimeofday(&end, NULL);
		testBTimes[i] = end.tv_sec - start.tv_sec;
		i++;
	}
	

	i = 0;
	
	//test C 100 times.	
	while (i < 100) {
		gettimeofday(&start, NULL);
		test_C();
		gettimeofday(&end, NULL);
		testCTimes[i] = end.tv_sec - start.tv_sec;
		i++;
	}
	

	i = 0;


	printf("The mean time test A: %f.\n", calculateMean(testATimes, 100));
	printf("The mean time test B: %f.\n", calculateMean(testBTimes, 100));
	printf("The mean time test C: %f.\n", calculateMean(testCTimes, 100));

	return 0;
}
