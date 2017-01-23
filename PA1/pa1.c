/*
 * pa1.c
 * -----
 *
 * ...
 *
 * 2017 James Lee, Emmanuel Thiessen
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

int i, N, L, M;
char *S;
int Stail = 0;
pthread_mutex_t Smutex = PTHREAD_MUTEX_INITIALIZER;

void guaranteedSleep(unsigned int s, unsigned int ns)
{
	struct timespec t = {s, ns};
	while (t.tv_nsec > 0 || t.tv_sec > 0) nanosleep(&t, &t);
}

void *threadfunc(void* arg)
{
	char character = (char)arg;

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

	// TODO: c0, c1, c2

	// Allocate space for the string on the stack (so we don't have to worry about deallocation/lack thereof)
	char s[L*M];
	S = s;

	// Same for the thread handles
	pthread_t threads[N];

	// ...
}
