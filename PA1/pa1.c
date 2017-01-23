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
#include <pthread.h>

int main(int argc, char ** argv)
{
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

	int
		i = atoi(argv[1]),
		N = atoi(argv[2]),
		L = atoi(argv[3]),
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

	if (L < 0) L = -L;
	if (M < 0) M = -M;

	pthread_t threads[N];

	// ...
}
