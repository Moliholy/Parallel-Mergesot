#include "ParallelMergesort.h"

/**
 * Number of threads that will be used. By default it is set to 1.
 */
int ParallelMergesort::numThreads = 1;

/**
 * Used to grant mutual exclusion when accesing (reading and writing)
 * the variable numThreads in the reserveThread method.
 */
pthread_mutex_t ParallelMergesort::mutex;

/**
 * This static method sorts the array arr from position 0 to last using numThreads threads.
 * It uses a improved version of Mergesort that only makes one memory reservation of
 * the whole array at the beginning of the algorithm.
 */
void ParallelMergesort::sort(int* arr, int last, int numThreads = 1) {
	/*the initial value of numThreads will be 1 less because we ARE actually
	 running in one thread, so we have numThreads-1 left */
	ParallelMergesort::numThreads = numThreads - 1;
	int* arr1 = arr;

	//copying the second array
	int *arr2 = new int[last];
	for (int i = 0; i < last; ++i)
		arr2[i] = arr1[i];

	//setting initial parameters for a full array
	Parameters p;
	p.arr1 = arr1;
	p.arr2 = arr2;
	p.first = 0;
	p.last = last - 1;
	p.h = 0;

	//calling mergesort to sort the algorithm
	mergesort(&p);

	//free copy-array
	delete[] arr2;
}

/**
 * Typical merge procedure of Mergesort. It "merges" the array, using arr1 or arr2 as
 * source and destination depending on the heigh of the tree.
 */
void ParallelMergesort::merge(int* arr1, int* arr2, int first, int last,
		int h) {
	int* source;
	int* destination;
	if (h % 2 == 0) {
		source = arr2;
		destination = arr1;
	} else {
		source = arr1;
		destination = arr2;
	}
	int middle = (first + last) / 2;
	for (int u = first, i = first, j = middle + 1; u <= last; u++) {
		if (i <= middle && j <= last) {
			if (source[i] < source[j]) {
				destination[u] = source[i++];
			} else {
				destination[u] = source[j++];
			}
		} else if (i > middle) {
			destination[u] = source[j++];
		} else {
			destination[u] = source[i++];
		}
	}
}

/**
 * Main Mergesort procedure. It firstly performs recursive calls to the same function and
 * after that calls the merge procedure. The parameters are passed by the params pointer.
 * It returns nothing meaningful, it is just a requirement for using the thread library.
 */
void* ParallelMergesort::mergesort(void* params) {
	Parameters* p = (Parameters*) params;

	if (p->last - p->first <= 0) {
		return 0;
	}

	int middle = (p->last + p->first) / 2;

	Parameters p2;
	p2.arr1 = p->arr1;
	p2.arr2 = p->arr2;
	p2.first = middle + 1;
	p2.last = p->last;
	p2.h = p->h + 1;

	Parameters p1;
	p1.arr1 = p->arr1;
	p1.arr2 = p->arr2;
	p1.first = p->first;
	p1.last = middle;
	p1.h = p->h + 1;

	//if there are threads avaliable...
	if (reserveThread()) {
		//we have more than one thread, so parallel execution is taking place

		//first create the thread that goes to the right branch in another thread
		pthread_t thread;
		pthread_create(&thread, NULL, mergesort, &p2);

		//and then THE SAME THREAD continues the execution on the left branch
		mergesort(&p1);

		//then it waits for the other thread
		pthread_join(thread, NULL);
		freeThread();
	} else {
		//if there are no threads avaliable then single execution
		mergesort(&p1);
		mergesort(&p2);
	}

	merge(p->arr1, p->arr2, p->first, p->last, p->h);
	return 0;
}

/**
 * Reserves a thread. This static method returns true if there were at least one thread
 * avaliable, and false otherwise. In order to read and write in a shared variable
 * this implementation uses the mutex attribute to guarantee the access in mutual
 * exclusion.
 */
bool ParallelMergesort::reserveThread() {
	bool toReturn = false;
	pthread_mutex_lock(&mutex);
	if (numThreads > 0) {
		--numThreads;
		toReturn = true;
	}
	pthread_mutex_unlock(&mutex);
	return toReturn;
}

/**
 * It frees one thread. As reserveThread(), it uses mutex operation to guarantee
 * the mutual exclusion while accessing a shared variable.
 */
void ParallelMergesort::freeThread() {
	pthread_mutex_lock(&mutex);
	++numThreads;
	pthread_mutex_unlock(&mutex);
}
