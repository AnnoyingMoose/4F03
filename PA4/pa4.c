/*
 * pa4.c
 * -----
 *
 * Written by:
 *   Emmanuel Thiessen (thiessen@mcmaster.ca, MSID#000945053)
 *   James Lee            (leejr@mcmaster.ca, MSID#001318125)
 *
 * for SFWR ENG 4F03 PA4 (Winter 2017)
 */

#include <stdlib.h>
#include <stdio.h>

#include <blur.h>

#include <string.h>
#include <mpi.h>

int main(int argc, char **argv)
{
	char greeting[MAX_STRING];
	int comm_sz;
	int my_rank;
	
	if (argc < 2)
	{
		fprintf(stderr, "No image specified\n");
		exit(1);
	}
	
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	if (my_rank != 0){
		fprintf(stderr, "Greetings from process %d of %d", my_rank, comm_sz);
		MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	}
	else{
		fprintf(stderr, "Greetings from process %d of %d", my_rank, comm_sz);
		for (int i = 1; i < comm_sz; i++){
			MPI_Recv(greeting, MAX_STRING, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}
	
	MPI_Finalize();
	return 0;

	// ...
}
