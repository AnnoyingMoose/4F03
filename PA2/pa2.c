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

// global variables to keep track of character numbers
int* numc0;
int* numc1;
int* numc2;
int cRequired = 0;

void threadFunc(int propertyIndex, int segmentLength, int numSegments, char c0, char c1, char c2, char *S, int *Stail, int *segmentsThatSatisfy)
{
	char character = 'a' + omp_get_thread_num();
	unsigned int rseed = (unsigned int)omp_get_thread_num();
	struct timespec sleepDuration = {0, 0};

	//fprintf(stderr, "Thread %d beginning string construction\n", omp_get_thread_num());

	// Construct the string
	while (*Stail < numSegments * segmentLength)
	{
		// Sleep for 100~500 ms
		sleepDuration.tv_nsec = (long int)(100000000.0 + rand_r(&rseed) * 400000000.0 / RAND_MAX);
		//fprintf(stderr, "Thread %d going to sleep for %ld ns\n", omp_get_thread_num(), sleepDuration.tv_nsec);
		nanosleep(&sleepDuration, NULL);
		//fprintf(stderr, "Thread %d waking up\n", omp_get_thread_num());

		// Append the current thread's character
		#pragma omp critical
		if (*Stail < numSegments * segmentLength){
			int currentSegment = *Stail/segmentLength;
			int currentIndex = *Stail;
			// if new segment then reset cRequired
			if (currentIndex%segmentLength==0)
				cRequired = 0;
			
			// add to the count of the character
			if (character == c0){
				numc0[currentSegment] ++;
				cRequired++;
			}
			else if (character == c1){
				numc1[currentSegment] ++;	
				cRequired+=2;				
			}
			else if (character == c2){
				numc2[currentSegment] ++;
				cRequired--;
			}
			S[(*Stail)++] = character;
			fprintf(stderr, "Add %c, tail at %d, cRequired = %d\n", character, *Stail, cRequired);
			
			int delete = 0;
			switch(propertyIndex){
				case 0:
					// numc0 + numc1 = numc2
					// this case is solved
					if (numc0[currentSegment] + numc1[currentSegment] > numc2[currentSegment])
						delete = 1;
					break;
				case 1:
					// numc0 + 2*numc1 = numc2
					// todo
					if (numc0[currentSegment] + 2*numc1[currentSegment] > numc2[currentSegment] ||
						cRequired > segmentLength-((currentIndex+1)%segmentLength)
					)
						delete = 1; 
					break;
				case 2:
					// numc0 * numc1 = numc2
					// to do
					if (numc0[currentSegment] * numc1[currentSegment] != numc2[currentSegment])
						delete = 1;
					break;
				case 3:
					// numc0 - numc1 = numc2
					// this case is solved
					if (numc0[currentSegment] < numc1[currentSegment] + numc2[currentSegment]
					|| ((currentIndex+1) % segmentLength==0 && numc0[currentSegment] > numc1[currentSegment] + numc2[currentSegment]))
						delete = 1;
					break;					
			}
			// if delete is true then delete the segment and reset
			if (delete){
				if (character == c0){
					numc0[currentSegment]--;
					cRequired--;
				}
				else if (character == c1){
					numc1[currentSegment]--;
					cRequired-=2;
				}
				else if (character == c2){
					numc2[currentSegment]--;
					cRequired++;					
				}
				(*Stail)--;
				fprintf(stderr, "Delete %c, tail at %d\n", character, *Stail);
			}
		}
	}

	int i, j;

	fprintf(stderr, "Thread %d beginning property checking\n", omp_get_thread_num());

	for (i = 0; i < numSegments / omp_get_num_threads(); ++i)
	{
		// Determine the starting index of the current segment
		int segmentStart = segmentLength * (i * omp_get_num_threads() + omp_get_thread_num());
		fprintf(stderr, "Thread %d checking segment %d\n", omp_get_thread_num(), segmentStart);

		// Verify the selected property

		int count[3] = {0, 0, 0};

		for (j = 0; j < segmentLength; ++j)
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

			#pragma omp critical
			++(*segmentsThatSatisfy);
		}
	}

	fprintf(stderr, "Thread %d finished.\n", omp_get_thread_num());
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

	char S[segmentLength * numSegments];
	int Stail = 0; // The first empty space in S (during the building phase)
	int segmentsThatSatisfy = 0; // Number of segments that have the selected property

	// variables that keep track of number of each character in each segment
	numc0 = malloc (numSegments);
	numc1 = malloc (numSegments);
	numc2 = malloc (numSegments);
	// set them to 0
	for (int i = 0; i < numSegments; i++){
		numc0[i] = 0;
		numc1[i] = 0;
		numc2[i] = 0;
	}

	fprintf(stderr, "Starting...\n");

	#pragma omp parallel num_threads(numThreads)
	threadFunc(propertyIndex, segmentLength, numSegments, c0, c1, c2, S, &Stail, &segmentsThatSatisfy);

	// Output the results; both to the terminal and the text file "out.txt"
	FILE *outFile = fopen("out.txt", "w");
	fprintf(outFile, "%s\n%d\n", S, segmentsThatSatisfy);
	fprintf(stdout,  "%s\n%d\n", S, segmentsThatSatisfy);
	fclose(outFile);

	for (int i = 0; i < numSegments; i++){
		fprintf(stdout,  "Numc0 segment %d has %d\n", i, numc0[i]);
	}
	for (int i = 0; i < numSegments; i++){
		fprintf(stdout,  "Numc1 segment %d has %d\n", i, numc1[i]);
	}
	for (int i = 0; i < numSegments; i++){
		fprintf(stdout,  "Numc2 segment %d has %d\n", i, numc2[i]);
	}
	
	return 0;
}
