#ifndef PARALLELMERGESORT_H_
#define PARALLELMERGESORT_H_

#include <pthread.h>


/**
 * This struct contains all necessary parameters to pass to
 * the mergesort method
 */
typedef struct {
	int* arr1;
	int* arr2;
	int first;
	int last;
	int h;
} Parameters;



/**
 * ParallelMergesor class sorts an array using a concrete number of threads
 */
class ParallelMergesort {

private:
	static int numThreads;
	static pthread_mutex_t mutex;

public:
	static void sort(int* arr, int last, int numThreads);

private:
	static void merge(int* source, int* destination, int first, int last,
			int h);
	static void* mergesort(void* params);
	static bool reserveThread();
	static void freeThread();
};

#endif /* PARALLELMERGESORT_H_ */
