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
#include <string.h>

int i, N, L, M;
char *S;
int Stail = 0;
pthread_mutex_t Smutex = PTHREAD_MUTEX_INITIALIZER;
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
int currentSegment = 0; // keep track of the next segment that needs to be checked, start at 1 for index 0 to 6
int segmentsThatSatisfy = 0; // number of segments that satisfy the conditions

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
		if (Stail < M * L){
			S[Stail++] = character;
		}

		pthread_mutex_unlock(&Smutex);
	}

	// store the segment locally and then use write lock
	char segment[L];
	pthread_rwlock_wrlock(&rwlock);
	int startIndex = currentSegment * L;
	strncpy(segment, S + startIndex, L);
	printf("%s\n", segment);
	currentSegment++; // increase the current segment
	printf("Hello from %c\n",character);
	// after incrementation, unlock
	pthread_rwlock_unlock(&rwlock);
	
	// Verify the selected property
	int satisfies = 0;
	// i = 0, occurences(c0) + occurences(c1) = occurences(c2)
	if (i == 0){
		satisfies = 1;
	}
	// i = 1, occurences(c0) + 2 x occurences(c1) = occurences(c2)
	else if (i == 1){
		
	}
	// i = 2, occurences(c0) x occurences(c1) = occurences(c2)
	else if (i == 2){
		
	}
	// i = 3, occurences(c0) - occurences(c1) = occurences(c2)
	else if (i == 3){
		
	}
	// if satisfies == 1 then get the lock again and update the value of segmentsThatSatisfy
	if (satisfies){
		pthread_rwlock_wrlock(&rwlock);
		segmentsThatSatisfy++;
		pthread_rwlock_unlock(&rwlock);
	}
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

	// write the results to a text file
	FILE *newFile = fopen("out.txt", "w");
	fputs(S, newFile);
	fprintf(newFile, "\n%d",segmentsThatSatisfy);
	fclose(newFile);
	
	return 0;
	// ...
	
}
