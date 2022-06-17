// mpicc Q4.c myhead.c -o Q4
// mpirun -np 3 ./Q4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <math.h>
#include "myhead.h"

#define LEN 5

void MY_Allgather(
	int *sendbuf,
	int sendcount,
	MPI_Datatype sendtype,
	int *recvbuf,
	int recvcount,
	MPI_Datatype recvtype,
	int iam,
	int np,
	MPI_Comm comm)
{
	int tag = 0;
	MPI_Status st;

	for(int i = 0; i < recvcount; i++){
		recvbuf[i] = sendbuf[i];
	}

	if(iam != 0){
		MPI_Send(sendbuf, LEN, MPI_INT, 0, tag, comm);
	}else{
		for(int i = 1; i < np; i++)
			MPI_Recv(recvbuf + i * LEN, LEN, MPI_INT, i, tag, comm, &st);
	}
	
	int src = (iam - 1 + np) % np;
	int dest = (iam + 1) & np;
	if(iam == 0){
		for(int i = 1; i < np; i++)
			MPI_Send(recvbuf, LEN * np, MPI_INT, i, tag, comm);
	}else{
		MPI_Recv(recvbuf, LEN * np, MPI_INT, 0, tag, comm, &st);
	}	
}


int main()
{
	int iam, np;
	MPI_Comm comm;
	MPI_Status st;

	MPI_Init(0, 0);
	MPI_Comm_dup(MPI_COMM_WORLD, &comm);
	MPI_Comm_rank(comm, &iam);
	MPI_Comm_size(comm, &np);

	int sendbuf[LEN];
	for(int i = 0; i < LEN; ++i) sendbuf[i] = iam * 10 + i;

	int recvbuf[LEN * np];
	for(int i = 0; i < LEN * np; i++) recvbuf[i] = 0;

	//memcpy(recvbuf + iam*LEN, sendbuf, sizeof(sendbuf));

	/*
	int tag = 0;
	if(iam != 0){
		MPI_Send(sendbuf, LEN, MPI_INT, 0, tag, comm);
	}else{
		for(int i = 1; i < np; i++)
			MPI_Recv(recvbuf + i * LEN, LEN, MPI_INT, i, tag, comm, &st);
	}
	
	int src = (iam - 1 + np) % np;
	int dest = (iam + 1) & np;
	if(iam == 0){
		for(int i = 1; i < np; i++)
			MPI_Send(recvbuf, LEN * np, MPI_INT, i, tag, comm);
	}else{
		MPI_Recv(recvbuf, LEN * np, MPI_INT, 0, tag, comm, &st);
	}
	*/
	MY_Allgather(sendbuf, LEN, MPI_INT, recvbuf, LEN, MPI_INT, iam, np, comm);

	printf("Proc %d:\n", iam);
	VecPrint(recvbuf, LEN*np, T_INT);

	MPI_Finalize();

	return 0;
}
