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
	int comm_sz = 1, my_rank = 0;

	MPI_Init(&argc, &argv);
	//atexit(MPI_Finalize);

	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	if (argc < 4)
	{
		if (my_rank == 0)
			fprintf(stderr, "Usage: mpirun -n <numProcesses> %s <radius> <sourceImage> <destImage>\n", argv[0]);

		exit(1);
	}
	
	int radius = atoi(argv[1]);
	Image *srcImage, *dstImage;

	srcImage = ImageRead(argv[2]);
	dstImage = ImageCreate(srcImage->width, srcImage->height);

	// divide work evenly among non main threads
	float work = (srcImage->height)/(comm_sz-1);
	if (my_rank != 0){
		fprintf(stderr, "Greetings from %d of %d\n", my_rank, comm_sz);	
		int i, j;
		for (i = radius; i < (srcImage->height) - radius; i++){
			for (j = radius; j < srcImage->width - radius; j++){
				blurPixel(srcImage, dstImage, radius, i, j);
			}
		}
	}
	else{
		fprintf(stderr, "Greetings from main\n");
		// main thread doesnt do anything (i think)
		
	}
	//blurImage(srcImage, dstImage, radius);

	ImageWrite(dstImage, argv[3]);
	
	MPI_Finalize();
	return 0;
}
