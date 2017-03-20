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
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

char** completedString;
int numThreads, segmentLength, numSegments;

// threads call this on the verify server to retrieve a segment to verify
char** RPC_GetSeg(int seg){
	char** string = "temp";
	return string;
}

// initialize the verify server, sending in required parameters
void RPC_InitVerifyServer(int N, int L, int M){
	numThreads = N;
	segmentLength = L;
	numSegments = M;
	// self setup
	int sockid, newsockid, portno, clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	sockid = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockid < 0){
		fprintf(stderr,"Could not open socket\n");
		exit(1);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = 20000;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	int status = bind(sockid, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	if (status < 0){ 
		fprintf(stderr,"Could not bind\n");
		exit(1);
	}
	listen(sockid,5);
	clilen = sizeof(cli_addr);
	newsockid = accept(sockid, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockid < 0){
		fprintf(stderr,"Could not accept\n");
		exit(1);
	}
	bzero(buffer,256);
	n = read(newsockid,buffer,255);
	if (n < 0){
		fprintf(stderr,"Could not read from socket\n");
		exit(1);
	}
	printf("Here is the message: %s\n",buffer);
	completedString = buffer;
	return 0; 
	
	// initialize UDP socket to recieve S from append server
}

int main(int argc, char ** argv)
{
}
