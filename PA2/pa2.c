/*
 * pa2.c
 * -----
 *
 * For SFWR ENG 4F03 PA2 (Winter 2017)
 *
 * Emmanuel Thiessen (thiessen@mcmaster.ca, MSID#000945053)
 * James Lee            (leejr@mcmaster.ca, MSID#001318125)
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#else
#define omp_get_thread_num()  0
#define omp_get_num_threads() 1
#endif

int *numc0, *numc1, *numc2, *numNC; // Arrays relating segments to character counts

int property(int index, int nc0, int nc1, int nc2)
{
	switch (index)
	{
	case  0: return nc0 + nc1   == nc2;
	case  1: return nc0 + nc1*2 == nc2;
	case  2: return nc0 * nc1   == nc2;
	case  3: return nc0 - nc1   == nc2;
	default: return 0;
	}
}

/**
 * Check if a property can be satisfied for a segment.
 *
 * @param propertyIndex: The index of the property to check
 * @param segmentLength: The length of the segment
 * @param nc0: The number of character c0 already in the segment
 * @param nc1: The number of character c1 already in the segment
 * @param nc2: The number of character c2 already in the segment
 * @param noncrit: The number of other characters already in the segment
 *
 * @return 1 if the property may yet be satisfied; 0 otherwise
 */
int isPossible(int propertyIndex, int segmentLength, int nc0, int nc1, int nc2, int noncrit)
{
	int tc0, tc1, tc2;

	for (tc0 = nc0; tc0 + nc1 + nc2 + noncrit <= segmentLength; ++tc0)
	for (tc1 = nc1; tc0 + tc1 + nc2 + noncrit <= segmentLength; ++tc1)
	for (tc2 = nc2; tc0 + tc1 + tc2 + noncrit <= segmentLength; ++tc2)
	{
		if (property(propertyIndex, tc0, tc1, tc2)) return 1;
	}

	return 0;
}

/**
 * Check if a property can be satisfied for a segment using only critical characters.
 *
 * @param propertyIndex: The index of the property to check
 * @param segmentLength: The length of the segment
 * @param nc0: The number of character c0 already in the segment
 * @param nc1: The number of character c1 already in the segment
 * @param nc2: The number of character c2 already in the segment
 *
 * @return 1 if the property may yet be satisfied; 0 otherwise
 */
int isPossibleWithoutNC(int propertyIndex, int segmentLength, int nc0, int nc1, int nc2)
{
	int tc0, tc1, tc2;

	for (tc0 = nc0; tc0 + nc1 + nc2 <= segmentLength; ++tc0)
	for (tc1 = nc1; tc0 + tc1 + nc2 <= segmentLength; ++tc1)
	{
		tc2 = segmentLength - tc0 - tc1;
		if (property(propertyIndex, tc0, tc1, tc2)) return 1;
	}

	return 0;
}

void threadFunc(int propertyIndex, int segmentLength, int numSegments, char c0, char c1, char c2, char *S, int *Stail, int *segmentsThatSatisfy)
{
	char chr = 'a' + omp_get_thread_num();
	unsigned int rseed = (unsigned int)omp_get_thread_num();
	struct timespec sleepDuration = {0, 0};
	int i, j, segSt, segCurr;

	// Construction phase

	while (*Stail < numSegments * segmentLength)
	{
		// Sleep for 100~500 ms
		sleepDuration.tv_nsec = (long int)(100000000.0 + rand_r(&rseed) * 400000000.0 / RAND_MAX);
		nanosleep(&sleepDuration, NULL);

		// Check if we can append the current thread's character
		// without violating the chosen property, and do so if so.
		#pragma omp critical
		if (*Stail < numSegments * segmentLength)
		{
			segCurr = *Stail / segmentLength;

			// Add to the character count:
			     if (chr == c0) ++numc0[segCurr];
			else if (chr == c1) ++numc1[segCurr];
			else if (chr == c2) ++numc2[segCurr];
			else                ++numNC[segCurr];

			if (omp_get_num_threads() == 3 ?
			    isPossibleWithoutNC(propertyIndex, segmentLength, numc0[segCurr], numc1[segCurr], numc2[segCurr]) :
			    isPossible         (propertyIndex, segmentLength, numc0[segCurr], numc1[segCurr], numc2[segCurr], numNC[segCurr]))
			{
				S[(*Stail)++] = chr;
				printf("%c", chr);
			}
			else
			{
				// Whoops, we can't add this character after all.
				// Undo the increment to the character count:
				     if (chr == c0) --numc0[segCurr];
				else if (chr == c1) --numc1[segCurr];
				else if (chr == c2) --numc2[segCurr];
				else                --numNC[segCurr];
			}
		}
	}

	// Property check phase

	for (i = 0; i < numSegments / omp_get_num_threads(); ++i)
	{
		// Determine the starting index of the current segment
		segSt = segmentLength * (i * omp_get_num_threads() + omp_get_thread_num());

		// Verify the selected property

		int count[3] = {0, 0, 0};

		for (j = 0; j < segmentLength; ++j)
		{
			char c = S[segSt+j];

			if (c == c0) ++count[0];
			if (c == c1) ++count[1];
			if (c == c2) ++count[2];
		}

		if (property(propertyIndex, count[0], count[1], count[2]))
		{
			// The current segment satisfies the given condition; increment the counter
			// Also make sure no-one else is trying to do so at the same time

			#pragma omp atomic
			++(*segmentsThatSatisfy);
		}
	}

	//~ fprintf(stderr, "Thread %d finished.\n", omp_get_thread_num());
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

	int
		propertyIndex = atoi(argv[1]),
		numThreads    = atoi(argv[2]),
		segmentLength = atoi(argv[3]),
		numSegments   = atoi(argv[4]);

	char
		c0 = argv[5][0],
		c1 = argv[6][0],
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

	if (numThreads == 3 ?
	    !isPossibleWithoutNC(propertyIndex, segmentLength, 0, 0, 0) :
	    !isPossible         (propertyIndex, segmentLength, 0, 0, 0, 0))
	{
		fprintf(stderr, "Error: selected property not possible for selected segment length.\n");
		exit(1);
	}

	char S[segmentLength * numSegments];
	int Stail = 0; // The first empty space in S (during the building phase)
	int segmentsThatSatisfy = 0; // Number of segments that have the selected property

	// variables that keep track of number of each character in each segment
	numc0 = calloc(numSegments, sizeof(int));
	numc1 = calloc(numSegments, sizeof(int));
	numc2 = calloc(numSegments, sizeof(int));
	numNC = calloc(numSegments, sizeof(int));

	//~ fprintf(stderr, "Starting...\n");

	#pragma omp parallel num_threads(numThreads)
	threadFunc(propertyIndex, segmentLength, numSegments, c0, c1, c2, S, &Stail, &segmentsThatSatisfy);

	printf("\n%d\n", segmentsThatSatisfy);

	// Output the results; both to the terminal and the text file "out.txt"
	FILE *outFile = fopen("out.txt", "w");
	fprintf(outFile, "%s\n%d\n", S, segmentsThatSatisfy);
	fclose(outFile);

	//~ int i;

	//~ for (i = 0; i < numSegments; i++)
	//~ {
		//~ fprintf(stderr, "Segment %d has the following character counts:\n", i);
		//~ fprintf(stderr, "c0: %d\nc1: %d\nc2: %d\nOther: %d\n\n", numc0[i], numc1[i], numc2[i], numNC[i]);
	//~ }

	free(numc0);
	free(numc1);
	free(numc2);
	free(numNC);

	return 0;
}
