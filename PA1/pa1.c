/*
 * pa1.c
 * -----
 *
 * ...
 *
 * For SFWR ENG 4F03 PA1 (Winter 2017)
 *
 * Emmanuel Thiessen (thiessen@mcmaster.ca, MSID#000945053)
 * James Lee            (leejr@mcmaster.ca, MSID#001318125)
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

int propertyIndex, numThreads, segmentLength, numSegments;

// The string to be built and analyzed, in its entirety
char *S;

// The first empty space in S (during the building phase)
int Stail = 0;

// Number of segments that have the selected property
int segmentsThatSatisfy = 0;

// The mutex to be used when changing S or the segmentsThatSatisfy variable
pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;

// The characters of interest during property checking
char c0, c1, c2;

/*
 * Sleep for s seconds and ns nanoseconds.
 * Is not interrupted by (benign) POSIX signals.
 * ns must be less than 1,000,000,000.
 */
void guaranteedSleep(unsigned int s, unsigned int ns)
{
	struct timespec t = {s, ns};
	while (t.tv_nsec > 0 || t.tv_sec > 0) nanosleep(&t, &t);
}

void *threadFunc(void* threadIndexArg)
{
	long threadIndex = (long)threadIndexArg;
	char character = 'a' + threadIndex;
	unsigned int rseed = (unsigned int)threadIndex;

	// Construct the string
	while (Stail < numSegments * segmentLength)
	{
		// Sleep for 100~500 ms
		guaranteedSleep(0, (unsigned int)(100000000.0 + rand_r(&rseed) * 400000000.0 / RAND_MAX));

		pthread_mutex_lock(&mx);

		// Append the current thread's character
		S[Stail++] = character;

		pthread_mutex_unlock(&mx);
	}

	for (int i = 0; i < numSegments / numThreads; ++i)
	{
		// Determine the starting index of the current segment
		int segmentStart = segmentLength * (i * numThreads + threadIndex);

		// Verify the selected property

		int count[3] = { 0, 0, 0 };

		for (int j = 0; j < segmentLength; ++j)
		{
			char c = S[segmentStart+j];

			if (c == c0) ++count[0];
			if (c == c1) ++count[1];
			if (c == c2) ++count[2];
		}

		int satisfies = 0;

		switch (propertyIndex)
		{
			case  0: satisfies = (count[0] + count[1]   == count[2]); break;
			case  1: satisfies = (count[0] + count[1]*2 == count[2]); break;
			case  2: satisfies = (count[0] * count[1]   == count[2]); break;
			default: satisfies = (count[0] - count[1]   == count[2]); break;
		}

		if (satisfies)
		{
			// The current segment satisfies the given condition; increment the counter
			// Also make sure no-one else is trying to do so at the same time

			pthread_mutex_lock(&mx);
			++segmentsThatSatisfy;
			pthread_mutex_unlock(&mx);
		}
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

	propertyIndex = atoi(argv[1]);
	numThreads    = atoi(argv[2]);
	segmentLength = atoi(argv[3]);
	numSegments   = atoi(argv[4]);
	c0 = argv[5][0];
	c1 = argv[6][0];
	c2 = argv[7][0];

	if (propertyIndex < 0 || propertyIndex > 3)
	{
		fprintf(stderr, "Error: property index must be in the range [0, 3] (got %d)\n", propertyIndex);
		exit(1);
	}

	if (numThreads < 3 || numThreads > 8)
	{
		fprintf(stderr, "Error: number of threads must be in the range [3, 8] (got %d)\n", numThreads);
		exit(1);
	}

	// We'll assume any non-negative values for L and M are valid
	if (segmentLength < 0) segmentLength = -segmentLength;
	if   (numSegments < 0)   numSegments = -numSegments;

	// Allocate space for the string on the stack (so we don't have to worry about deallocation/lack thereof)
	char s[segmentLength * numSegments];
	S = s;

	// Same for the thread handles
	pthread_t threads[numThreads];

	fprintf(stderr, "Creating threads... ");

	// Initialize and run each thread
	for (long threadIndex = 0; threadIndex < numThreads; ++threadIndex)
		pthread_create(&threads[threadIndex], NULL, threadFunc, (void*)threadIndex);

	fprintf(stderr, "All threads created.\nWaiting for completion... ");

	// Wait for the threads to complete
	for (long threadIndex = 0; threadIndex < numThreads; ++threadIndex)
		pthread_join(threads[threadIndex], NULL);

	fprintf(stderr, "All threads completed.\n");

	// Output the results; both to the terminal and the text file "out.txt"
	FILE *outFile = fopen("out.txt", "w");
	fprintf(outFile, "%s\n%d", S, segmentsThatSatisfy);
	fprintf(stdout,  "%s\n%d", S, segmentsThatSatisfy);
	fclose(outFile);

	return 0;
}
