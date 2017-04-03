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
#include <mpi.h>

#include "blur.h"

int main(int argc, char **argv)
{
	int comm_sz, my_rank;

	MPI_Init(&argc, &argv);
	atexit(MPI_Finalize);

	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	if (argc < 4)
	{
		if (my_rank == 0)
			fprintf(stderr, "Usage: %s radius source dest\n", argv[0]);

		exit(1);
	}

	// ...

	return 0;
}
