/*
 * server_verify.c
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

char** completedString;

// threads call this on the verify server to retrieve a segment to verify
char** RPC_GetSeg(int seg){
	char** string = "temp";
	return string;
}

// initialize the verify server, sending in required parameters
void RPC_InitVerifyServer(int numThreads, int segmentLength, int numSegments){
	// self setup
	
	// initialize UDP socket to recieve S from append server
}
