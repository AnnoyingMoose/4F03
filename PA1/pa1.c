/*
 * pa1.c
 * -----
 *
 * ...
 *
 * 2017
 * Emmanuel Thiessen
 * James Lee 1318125
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

int i, N, L, M;
char *S;
int Stail = 0;
pthread_mutex_t Smutex = PTHREAD_MUTEX_INITIALIZER;
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void guaranteedSleep(unsigned int s, unsigned int ns)
{
	struct timespec t = {s, ns};
	while (t.tv_nsec > 0 || t.tv_sec > 0) nanosleep(&t, &t);
}

void *threadfunc(void* arg)
{
	char character = (char*)arg;
	// Construct the string
	while (Stail < M * L)
	{
		// Sleep for 100~500 ms
		guaranteedSleep(0, 100000000 + rand() % 400000000);

		pthread_mutex_lock(&Smutex);

		// Append the current thread's character
		// (if the string isn't already fully built)
		if (Stail < M * L)
			S[Stail++] = character;

		pthread_mutex_unlock(&Smutex);
	}

	// Verify the selected property
	// TODO
	pthread_rwlock_rdlock(&rwlock);
	printf("Hello from %c\n",character);
	pthread_rwlock_unlock(&rwlock);

	return NULL;
}

int main(int argc, char ** argv)
{
	// Command-line argument sanity-checking

	if (argc != 8)
	{
		fprintf(stderr, "Usage: %s i N L M c0 c1 c2\n", argv[0]);
		fprintf(stderr, "i: index of the property which each segment needs to satisfy\n");
		fprintf(stderr, "N: number of threads\n");
		fprintf(stderr, "L: length of each segment\n");
		fprintf(stderr, "M: number of segments to generate\n");
		fprintf(stderr, "c0, c1, c2: letters to use in property check\n");
		exit(1);
	}

	i = atoi(argv[1]);
	N = atoi(argv[2]);
	L = atoi(argv[3]);
	M = atoi(argv[4]);

	if (i < 0 || i > 3)
	{
		fprintf(stderr, "Error: property index must be in the range [0, 3] (got %i)", i);
		exit(1);
	}

	if (N < 3 || N > 8)
	{
		fprintf(stderr, "Error: number of threads must be in the range [3, 8] (got %i)", N);
		exit(1);
	}

	// We'll assume any non-negative values for L and M are valid
	if (L < 0) L = -L;
	if (M < 0) M = -M;
	
	// Allocate space for the string on the stack (so we don't have to worry about deallocation/lack thereof)
	char s[L*M];
	S = s;

	// Same for the thread handles
	long threadCount; // long in case of 64 bit system
	int numThreads = N;
	pthread_t* threads;
	threads = malloc (numThreads * sizeof(pthread_t));
	
	// create the threads, let them run the function threadfunc once created,
	// and pass the arguments from the input arguments.
	// strings are actually character arrays, so to get the character 'b' from the arguments we 
	// must do argv[argument index][0]
	for (threadCount = 0; threadCount < N; threadCount++){
		pthread_create(&threads[threadCount], NULL, threadfunc, argv[threadCount+5][0]);
	}
	printf("Hello from the main thread\n");
	// join the threads once they complete 
	for (threadCount = 0; threadCount < N; threadCount++){
		pthread_join(threads[threadCount], NULL);
	}
	// free memory
	free (threads);

	// write the result to a text file
	FILE *newFile = fopen("out.txt", "w");
	fprintf(newFile, "Hello");
	fclose(newFile);
	
	return 0;
	// ...
	
}
