/*
 * server_append.c
 * ---------------
 *
 * For SFWR ENG 4F03 PA3 (Winter 2017)
 *
 * Emmanuel Thiessen (thiessen@mcmaster.ca, MSID#000945053)
 * James Lee            (leejr@mcmaster.ca, MSID#001318125)
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

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
void RPC_InitAppendServer(int propertyIndex, int segmentLength, int numSegments, char c0, char c1, char c2, char* hostName){
	// self setup
	int sockid, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    portno = 20000;
    sockid = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockid < 0){
		fprintf(stderr,"Could not open socket\n");
		exit(1);
	}
    server = gethostbyname(*hostName);
    if (server == NULL) {
        fprintf(stderr,"No such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
	int status = connect(sockid,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    if (status < 0){
		fprintf(stderr,"Could not connect\n");
		exit(1);
	}
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockid,buffer,strlen(buffer));
    if (n < 0){
		fprintf(stderr, "Could not write to socket\n");
		exit(1);
	}
    return 0;

}

int main(int argc, char ** argv)
{
}
