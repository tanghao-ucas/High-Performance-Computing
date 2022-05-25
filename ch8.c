#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "myhead.h"

int main(int argc, char** argv)
{
	int iam, np;
	MPI_Comm comm;
	MPI_Status st;

	MPI_Init(&argc, &argv);
	MPI_Comm_dup(MPI_COMM_WORLD, &comm);
	MPI_Comm_rank(comm, &iam);
	MPI_Comm_size(comm, &np);
	
	int len = 6;
	int m = len / np;
	int A[len * len];
	int X[len];
	int tmp_a[m * len];
	int tmp_x[m];
	int tmp_c[len];
	int B[len];
	for(int i = 0; i < len; i++){
		for(int j = 0; j < len; j++)
			A[i * len + j] = 0;
	}
	for(int i = 0; i < len; i++) {X[i] = 0; B[i] = 0; tmp_c[i] = 0;}

	if(iam == 0){
		for(int i = 0; i < len; i++){
			for(int j = 0; j < len; j++)
				A[j * len + i] = i * len + j;
			X[i] = i + 1;
		}
		for(int i = 0; i < len; i++) B[i] = i;
		
		for(int i = 1; i < np; i++){
			MPI_Send(&A[i * m * len], m * len, MPI_INT, i, 0, comm);
			MPI_Send(&X[i * m], m, MPI_INT, i, 99, comm);
		}

		for(int i = 0; i < m; i++)
			for(int j = 0; j < len; j++)
				tmp_c[j] += (A[i * len + j] * X[i]);

		for(int i = 0; i < len; i++) tmp_c[i] += B[i];
	}
	else{
		MPI_Recv(tmp_a, m * len, MPI_INT, 0, 0, comm, &st);
		MPI_Recv(tmp_x, m, MPI_INT, 0, 99, comm, &st);
		for(int i = 0; i < m; i++)
			for(int j = 0; j < len; j++)
				tmp_c[j] += (tmp_a[i * len + j] * tmp_x[i]);
	}

	MPI_Reduce(tmp_c, X, len, MPI_INT, MPI_SUM, 0, comm);

	if(iam == 0) VecPrint(X, len, sizeof(int));

	MPI_Finalize();

	return 0;
}
