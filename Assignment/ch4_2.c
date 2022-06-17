#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "myhead.h"

#define N 6

int main()
{
	int iam, np;
	MPI_Comm comm;
	MPI_Status st;

	MPI_Init(NULL, NULL);
	MPI_Comm_dup(MPI_COMM_WORLD, &comm);
	MPI_Comm_rank(comm, &iam);
	MPI_Comm_size(comm, &np);

	int A[N][N];
	int X[N];
	int B[N];
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++)
			A[i][j] = 0;
		X[i] = 0;
		B[i] = 0;
	}

	int mp = N / np; 	// mp: rows in each proc
	int tmp_A[N * mp];
	for(int i = 0; i < N * mp; i++) tmp_A[i] = 0;
	int tmp_B[mp];
	for(int i = 0; i < mp; i++) tmp_B[i] = 0;

	if(iam == 0){
		for(int i = 0; i < N; i++){
			for(int j = 0; j < N; j++)
				A[i][j] = i * 6 + j;
			X[i] = i + 1;
			B[i] = i + 1;
		}
		for(int i = 1; i < np; i++){
			MPI_Send(A[i * mp], N * mp, MPI_INT, i, 0, comm);
			MPI_Send(&B[i * mp], mp, MPI_INT, i, 99, comm);
		}
	}
	else{
		MPI_Recv(tmp_A, N * mp, MPI_INT, 0, 0, comm, &st);
		MPI_Recv(tmp_B, mp, MPI_INT, 0, 99, comm, &st);
	}
	MPI_Bcast(X, N, MPI_INT, 0, comm);

	if(iam == 0){
		for(int i = 0; i < mp; i++){
			tmp_B[i] = B[i];
			for(int j = 0; j < N; j++)
				tmp_B[i] += A[i][j] * X[j];
		}
	}
	else{
		for(int i = 0; i < mp; i++)
		       for(int j = 0; j < N; j++)
				tmp_B[i] += tmp_A[i * N + j] * X[j];		
	}	
	
	MPI_Gather(tmp_B, mp, MPI_INT, X, mp, MPI_INT, 0, comm);

	if(iam == 0)
		VecPrint(X, N, sizeof(int));

	MPI_Finalize();

	return 0;
}

	
	

