/*
 * server_append.c
 * ---------------
 *
 * For SFWR ENG 4F03 PA3 (Winter 2017)
 *
 * Emmanuel Thiessen (thiessen@mcmaster.ca, MSID#000945053)
 * James Lee            (leejr@mcmaster.ca, MSID#001318125)
 */

#include "server_append.h"
#include "property.h"

char c[3], *S = NULL, *verifyHost = NULL;

int
	propertyIndex,
	numThreads,
	numSegments,
	segmentLength,
	tail, *ccount[4],
	appendReturnCode;

int *rpc_append_1_svc(char *chr, struct svc_req *req)
{
	appendReturnCode = -1;

	if (tail < numSegments * segmentLength)
	{
		int i, currSeg = tail / segmentLength;

		// Add to the character count:
			 if (chr == c[0]) ++ccount[0][currSeg];
		else if (chr == c[1]) ++ccount[1][currSeg];
		else if (chr == c[2]) ++ccount[2][currSeg];
		else                  ++ccount[3][currSeg];

		if (numThreads == 3 ?
		    isPossible  (propertyIndex, segmentLength, ccount[0][currSeg], ccount[1][currSeg], ccount[2][currSeg]) :
		    isPossibleNC(propertyIndex, segmentLength, ccount[0][currSeg], ccount[1][currSeg], ccount[2][currSeg], ccount[3][currSeg]))
		{
			S[tail++] = chr;
			appendReturnCode = 0;
		}
		else
		{
			// Whoops, we can't add this character after all.
			// Undo the increment to the character count:
				 if (chr == c[0]) --ccount[0][currSeg];
			else if (chr == c[1]) --ccount[1][currSeg];
			else if (chr == c[2]) --ccount[2][currSeg];
			else                  --ccount[3][currSeg];
		}
	}

	return &appendReturnCode;
}

void *rpc_initappendserver_1_svc(char **argp, struct svc_req *req)
{
	int i;

	// Unmarshal arguments

	char *args = *argp;

	propertyIndex = (int)args[0];
	numThreads    = (int)args[1];
	numSegments   = (int)args[2];
	segmentLength = (int)args[3];

	for (i = 0; i < 3; i++)
		c[i] = args[i+4];

	free(verifyHost);
	verifyHost = strdup(args+7);

	for (i = 0; i < 4; i++)
	{
		free(ccount[i]);
		ccount[i] = calloc(numSegments, sizeof(int));
	}

	free(S);
	S = malloc(numSegments * segmentLength);

	tail = 0;

	return NULL;
}
