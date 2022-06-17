#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "myhead.h"

#define ETYPE double
#define len 6

int getsize(MPI_Datatype type)
{
	int s;
	switch(type){
		case MPI_INT: s = 4; break;
		case MPI_DOUBLE: s = 8; break;
		case MPI_CHAR: s = 1; break;
		default: s = 0; break;
	}
	return s;
}

void alltoall(void *sendbuf,
	      int sendcount,
	      MPI_Datatype sendtype,
	      void *recvbuf,
	      int recvcount,
	      MPI_Datatype recvtype, 
	      MPI_Comm comm)
{
	int iam, np;
	MPI_Status st;
	MPI_Comm_rank(comm, &iam);
	MPI_Comm_size(comm, &np);

	int next, front;
	void *nptr, *fptr; 
	int sendsize = getsize(sendtype), recvsize = getsize(recvtype);	
	
	for(next = (iam + 1) % np; next != iam; next = (next + 1) % np){
		nptr = sendbuf + next * sendcount * sendsize;
		MPI_Send(nptr, sendcount, sendtype, next, 0, comm);
	}
	for(front = (iam - 1 + np) % np; front != iam; front = (front - 1 + np) % np){
		fptr = recvbuf + front * recvcount * recvsize;
		MPI_Recv(fptr, recvcount, recvtype, front, 0, comm, &st);
	}
}


int main(int argc, char **argv)
{
	int iam, np;
	MPI_Comm comm;
	MPI_Status st;

	MPI_Init(&argc, &argv);
	MPI_Comm_dup(MPI_COMM_WORLD, &comm);
	MPI_Comm_rank(comm, &iam);
	MPI_Comm_size(comm, &np);

	ETYPE data[len];
	ETYPE recv[len];
	int blocklen = len / np;
	for(int i = 0; i < len; i++) data[i] = iam * 10 + i;
	for(int i = 0; i < len; i++) recv[i] = 0;
	for(int i = iam * blocklen; i < (iam + 1) * blocklen; i++) recv[i] = data[i];

	alltoall(data, blocklen, MPI_DOUBLE, recv, blocklen, MPI_DOUBLE, comm);
	
	printf("Proc %d: ", iam);	
	VecPrint(recv, len, sizeof(ETYPE));
	
	MPI_Finalize();
	return 0;
}
