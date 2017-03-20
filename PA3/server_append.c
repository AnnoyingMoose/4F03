/*
 * pa3.c
 * -----
 *
 * For SFWR ENG 4F03 PA3 (Winter 2017)
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

char** currentString;
int currentIndex = 0;

// threads call this to try and append a character
int RPC_Append(char c){
	currentString[currentIndex++] = c;
	// return 0 if appending success
	return 0;
	// return -1 if S is complete
}

// initialize the append server, sending in required parameters
void RPC_InitAppendServer(int propertyIndex, int segmentLength, int numSegments, char c0, char c1, char c2, char hostName[]){
	// self setup
}
