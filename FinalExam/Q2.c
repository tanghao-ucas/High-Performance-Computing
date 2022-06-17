// mpicc Q2.c myhead.c -o Q2
// mpirun -np 2 ./Q2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "myhead.h"

#define LEN 6	// 矩阵总列数
#define m 2

int main(int argc, char** argv)
{
	int iam, np;
	MPI_Comm comm;
	MPI_Status st;

	MPI_Init(&argc, &argv);
	MPI_Comm_dup(MPI_COMM_WORLD, &comm);
	MPI_Comm_rank(comm, &iam);
	MPI_Comm_size(comm, &np);

	int data[LEN][LEN];
	for(int i = 0; i < LEN; i++)
		for(int j = 0; j < LEN; j++)
			data[i][j] = 0;
	
	MPI_Datatype submat, submat2;
	
	// (a) A00 or A20
	
	MPI_Type_vector(m, m, LEN, MPI_INT, &submat);
	MPI_Type_commit(&submat);

	MPI_Aint disp = (2 * m * LEN) * sizeof(int);
	MPI_Type_create_resized(submat, 0, disp, &submat2);
	MPI_Type_commit(&submat2);
	
	// (b) A00 and A20
	/*
	int count = 3 * m;
	int blocklen[3 * m];
	for(int i = 0; i < 3 * m; i++) blocklen[i] = ((i < 2 * m) && (i >= m)) ? 0 : m;
	int disp[3 * m];
	for(int i = 0 ; i < 3 * m; i++) disp[i] = LEN * i;
	
	MPI_Type_indexed(count, blocklen, disp, MPI_INT, &submat2);
	MPI_Type_commit(&submat2);
	*/	
	
	if(iam == 0){
		for(int i = 0; i < LEN; i++)
			for(int j = 0; j < LEN; j++)
				data[i][j] = LEN * i + j;

		MPI_Send(data, 1, submat2, 1, 0, comm);
	}
	else{
		MPI_Recv(data, 1, submat2, 0, 0, comm, &st);
		for(int i = 0; i < LEN; i++)
			VecPrint(data[i], LEN, T_INT);
	}

	MPI_Finalize();
	
	return 0;
}

		
