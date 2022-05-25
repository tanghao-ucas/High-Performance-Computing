#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "myhead.h"

int main(int argc, char **argv)
{
	int iam, np;
	MPI_Comm comm;
	MPI_Status st;

	MPI_Init(&argc, &argv);
	MPI_Comm_dup(MPI_COMM_WORLD, &comm);
	MPI_Comm_rank(comm, &iam);
	MPI_Comm_size(comm, &np);

	int data[6];
	for(int i = 0; i < 6; i++) data[i] = 0;

	if(iam == 0){
		for(int i = 0; i < 6; i++)
			data[i] = i + 1;
	}

	int m = 6 / np;
	int recv[m];
	MPI_Scatter(data, m, MPI_INT, recv, m, MPI_INT, 0, comm);

	MPI_Allgather(recv, m, MPI_INT, data, m, MPI_INT, comm);
	printf("Proc %d: ", iam);
	VecPrint(data, 6, sizeof(int));

	MPI_Finalize();

	return 0;
}

