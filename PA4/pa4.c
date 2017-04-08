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
	double startTime, endTime;
	startTime = MPI_Wtime();
	
	int myRank = 0, numProcesses = 1, source;
	double totalTime;

	MPI_Init(&argc, &argv);
	atexit((void(*)(void))MPI_Finalize);

	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);


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

	endTime = MPI_Wtime();
	double diff = 1000*(endTime-startTime);

	if (myRank != 0){
		MPI_Send(&diff, 1, MPI_INTEGER, 0, 0, MPI_COMM_WORLD);	
	}
	else{
		totalTime = diff;
		for(source = 1; source < numProcesses; source++){
			MPI_Recv(&diff, 1, MPI_INTEGER, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			totalTime += diff;
		}
		fprintf(stderr, "Average time taken was %f milliseconds\n", totalTime/numProcesses); 
	}
	return 0;
}
