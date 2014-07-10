#include <iostream>
#include <cstdlib>
#include <sys/time.h>
#include "ParallelMergesort.h"

using namespace std;

/**
 * Initializes the array with values between 0 and 99
 */
void generateRandomArray(int* array, int size) {
	for (int i = 0; i < size; ++i) {
		array[i] = rand() % 100;
	}
}

/**
 * Prints the array
 */
void printArray(int* array, int size) {
	for (int i = 0; i < size; ++i) {
		cout << array[i] << " ";
	}
	cout << endl;
}

/**
 *  Returns the current wall clock time in seconds.
 *  Copied from assignment 5.
 */
static double gettime(void) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + 1e-6 * tv.tv_usec;
}

/* main()
 *
 * Usage: ./program size numThreads
 *
 * size: the number of elements in the array
 *
 * numThreads: the number of threads used by the algorithm
 *
 */
int main(int argc, char *argv[]) {
	if (argc < 3) {
		cout << "Invalid arguments. Syntax: program size numThreads" << endl;
	}

	srand(time(NULL));
	int size = atoi(argv[1]);
	int numThreads = atoi(argv[2]);

	int* array = new int[size];

	//generating a random array
	generateRandomArray(array, size);

	//prints the array before the execution
	//printArray(array, size);

	double t = gettime();
	//calls the static method to sort the array
	ParallelMergesort::sort(array, size, numThreads);

	double totalTime = gettime() - t;

	//... and it prints again the array after the execution, sorted this time
	//printArray(array, size);

	//total time
	cout << totalTime << endl;

	return 0;
}
