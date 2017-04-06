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

#include "ppmFile.h"
#include "blur.h"

int main(int argc, char **argv)
{
	int myRank = 0;

	MPI_Init(&argc, &argv);
	atexit((void(*)(void))MPI_Finalize);

	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	if (argc < 4)
	{
		if (myRank == 0)
			fprintf(stderr, "Usage: mpirun -n <numProcesses> %s <radius> <sourceImage> <destImage>\n", argv[0]);

		exit(1);
	}

	int radius = atoi(argv[1]);
	Image *srcImage, *dstImage;

	srcImage = ImageRead(argv[2]);
	dstImage = ImageCreate(srcImage->width, srcImage->height);

	blurImage(srcImage, dstImage, radius);

	if (myRank == 0)
		ImageWrite(dstImage, argv[3]);

	return 0;
}
