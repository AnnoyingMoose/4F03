/*
 * client.c
 * --------
 *
 * For SFWR ENG 4F03 PA3 (Winter 2017)
 *
 * Emmanuel Thiessen (thiessen@mcmaster.ca, MSID#000945053)
 * James Lee            (leejr@mcmaster.ca, MSID#001318125)
 */

#include "property.h"
#include "server_append.h"
#include "server_verify.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#else
#define omp_get_thread_num()  0
#define omp_get_num_threads() 1
#endif

CLIENT *acl, *vcl;
int segmentsThatSatisfy = 0; // Number of segments that have the selected property

void InitAppendServer(int propertyIndex, int segmentLength, int numSegments, char c0, char c1, char c2, char *verifyHost)
{
	char marshal[strlen(verifyHost) + 6];
	marshal[0] = (char)propertyIndex;
	marshal[1] = (char)segmentLength;
	marshal[2] = (char)numSegments;
	marshal[3] = c0;
	marshal[4] = c1;
	marshal[5] = c2;
	strcpy(marshal+6, verifyHost);

	rpc_initappendserver_1(&marshal, acl);
}

void InitVerifyServer(int numThreads, int segmentLength, int numSegments)
{
	char marshal[3] = {(char)numThreads, (char)segmentLength, (char)numSegments};
	rpc_initverifyserver_1(&marshal, vcl);
}

void constructAndVerify(int propertyIndex, int segmentLength, char c0, char c1, char c2)
{
	char chr = 'a' + omp_get_thread_num();
	unsigned int rseed = (unsigned int)omp_get_thread_num();
	struct timespec sleepDuration = {0, 0};
	int i, lastAppend, localGoodSegments, tid = omp_get_thread_num();
	char *segment;

	// Construction phase

	do
	{
		// Sleep for 100~500 ms
		sleepDuration.tv_nsec = (long int)(100000000.0 + rand_r(&rseed) * 400000000.0 / RAND_MAX);
		nanosleep(&sleepDuration, NULL);

		lastAppend = *rpc_append_1(&chr, acl);
	}
	while (lastAppend == 0);

	// Property check phase

	while (1)
	{
		segment = *rpc_getseg_1(&tid, vcl);

		if (segment == NULL || segment[0] == '-') break;

		// Verify the selected property

		int count[3] = {0, 0, 0};

		for (i = 0; i < segmentLength; ++i)
		{
			     if (segment[i] == c0) ++count[0];
			else if (segment[i] == c1) ++count[1];
			else if (segment[i] == c2) ++count[2];
		}

		if (property(propertyIndex, count[0], count[1], count[2]))
		{
			// The current segment satisfies the given condition; increment the counter
			++localGoodSegments;
		}
	}

	#pragma omp atomic
	segmentsThatSatisfy += localGoodSegments;

	//~ fprintf(stderr, "Thread %d finished.\n", omp_get_thread_num());
}

int main(int argc, char **argv)
{
	// Command-line argument sanity-checking

	if (argc != 10)
	{
		fprintf(stderr, "Usage: %s i N L M c0 c1 c2 hn1 hn2\n", argv[0]);
		fprintf(stderr, "i: index of the property which each segment needs to satisfy\n");
		fprintf(stderr, "N: number of threads\n");
		fprintf(stderr, "L: length of each segment\n");
		fprintf(stderr, "M: number of segments to generate\n");
		fprintf(stderr, "c0, c1, c2: letters to use in property check\n");
		fprintf(stderr, "hn1, hn2: host names of append and verify servers, respectively.\n");
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
		c2 = argv[7][0],
		*appendHost = argv[8],
		*verifyHost = argv[9];

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

	//~ fprintf(stderr, "Starting...\n");

	// Create client objects
	acl = clnt_create(appendHost, APPENDPROG, APPENDVERS, "tcp");

	if (acl == NULL)
	{
		fprintf(stderr, "Failed to contact Append server.\n");
		exit(2);
	}

	vcl = clnt_create(verifyHost, VERIFYPROG, VERIFYVERS, "tcp");

	if (vcl == NULL)
	{
		fprintf(stderr, "Failed to contact Verify server.\n");
		exit(2);
	}

	// Send initialization signals to servers
	InitAppendServer(propertyIndex, segmentLength, numSegments, c0, c1, c2, verifyHost);
	InitVerifyServer(numThreads, segmentLength, numSegments);

	#pragma omp parallel num_threads(numThreads)
	constructAndVerify(propertyIndex, segmentLength, c0, c1, c2);

	printf("\n%d\n", segmentsThatSatisfy);

	// Output the results; both to the terminal and the text file "out.txt"
	FILE *outFile = fopen("out.txt", "w");
	fprintf(outFile, "%s\n%d\n", *rpc_getstring_1(NULL, vcl), segmentsThatSatisfy);
	fclose(outFile);

	//~ int i;

	//~ for (i = 0; i < numSegments; i++)
	//~ {
		//~ fprintf(stderr, "Segment %d has the following character counts:\n", i);
		//~ fprintf(stderr, "c0: %d\nc1: %d\nc2: %d\nOther: %d\n\n", numc0[i], numc1[i], numc2[i], numNC[i]);
	//~ }

	return 0;
}
