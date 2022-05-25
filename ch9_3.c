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

	int data[N][N];
	for(int i = 0; i < N; i++)
		for(int j = 0; j < N; j++)
			data[i][j] = 0;

	int m = 2;
	int sendcount = N / m;

	int count = m;
	int blocklen[m];
	int disp[m];
	MPI_Datatype lowmat;

	for(int i = 0; i < m; i++){
		blocklen[i] = i + 1;
		disp[i] = i * N;
	}
	
	MPI_Type_indexed(count, blocklen, disp, MPI_INT, &lowmat);
	MPI_Type_commit(&lowmat);

	count = 2;
	int blocklength[2] = {1, 1};
	MPI_Aint displace[2] = {0, sizeof(int) * (m * N + m)};
	MPI_Datatype types[2] = {lowmat, MPI_UB};
	MPI_Datatype diagmat;

	MPI_Type_struct(count, blocklength, displace, types, &diagmat);
	MPI_Type_commit(&diagmat);

	if(iam == 0){
		for(int i = 0; i < N; i++)
			for(int j = 0; j < N; j++)
				data[i][j] = i * N + j + 1;
		
		for(int i = 0 ; i < N; i++)
			VecPrint(data[i], N, sizeof(int));
		
		MPI_Send(data, sendcount, diagmat, 1, 0, comm);
	}
	else{
		MPI_Recv(data, sendcount, diagmat, 0, 0, comm, &st);
		for(int i = 0 ; i < N; i++)
			VecPrint(data[i], N, sizeof(int));
	}

	MPI_Finalize();

	return 0;

}
	
	

