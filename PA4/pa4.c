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
	MPI_Init(&argc, &argv);
	atexit(MPI_Finalize);

	if (argc < 4)
	{
		fprintf(stderr, "Usage: %s radius source dest\n", argv[0]);
		exit(1);
	}

	// ...

	return 0;
}
