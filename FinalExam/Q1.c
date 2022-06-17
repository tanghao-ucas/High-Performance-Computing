// mpicc Q1.c myhead.c -o Q1
// mpirun -np 16 ./Q1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "myhead.h"

// (a)
void MPI_Comm_split2d(MPI_Comm comm,         
		      int iam,
		      int np,
		      int p,          
		      int q,          
		      MPI_Comm *row_comm,   
		      MPI_Comm *col_comm)
{
	if(np < p * q || np != p * q) return; 
	int row_color = iam / q;
	MPI_Comm_split(comm, row_color, iam, row_comm);
	
	int col_color = iam % q;
	MPI_Comm_split(comm, col_color, iam, col_comm);
}

// (c)
void MPI_Bcast2d(void* databuf,
		 int count,
		 MPI_Datatype datatype,
		 int row_root,
		 int col_root,
		 MPI_Comm row_comm,
		 MPI_Comm col_comm)
{
	MPI_Bcast(databuf, count, datatype, col_root, row_comm);
	MPI_Bcast(databuf, count, datatype, row_root, col_comm);
}

int main()
{
	int iam, np;
	MPI_Comm comm;
	MPI_Status st;

	MPI_Init(NULL, NULL);
	MPI_Comm_dup(MPI_COMM_WORLD, &comm);
	MPI_Comm_rank(comm, &iam);
	MPI_Comm_size(comm, &np);

	// assume p = 4 at initial
	int p = 4, q = np/p;  

	MPI_Comm row_comm, col_comm;

	MPI_Comm_split2d(comm, iam, np, p, q, &row_comm, &col_comm);
	
	int row_rank, row_size;
	MPI_Comm_rank(row_comm, &row_rank);
	MPI_Comm_size(row_comm, &row_size);

	int col_rank, col_size;
	MPI_Comm_rank(col_comm, &col_rank);
	MPI_Comm_size(col_comm, &col_size);

	int data = iam + 1;		// 整形数据
	int recv = -1;
	int tag = 99;
	/*
	int next = (row_rank + 1) % row_size;
	int front = (row_rank - 1 + row_size) % row_size;
	
	if(row_rank == 0){
		MPI_Send(&data, 1, MPI_INT, next, tag, row_comm);
		MPI_Recv(&recv, 1, MPI_INT, front, tag, row_comm, &st);
	}
	else{
		MPI_Recv(&recv, 1, MPI_INT, front, tag, row_comm, &st);
		MPI_Send(&data, 1, MPI_INT, next, tag, row_comm);
	}
	*/
	MPI_Bcast2d(&data, 1, MPI_INT, 0, 0, row_comm, col_comm);	// 从（0，0）发送数据给其他块
	
	int send = 0;

	if(iam == 0){
		printf("Proc %d: %d\n", iam, data);
		MPI_Send(&send, 1, MPI_INT, iam + 1, tag, comm);
	}
	else if(iam == np - 1){
		MPI_Recv(&send, 1, MPI_INT, iam - 1, tag, comm, &st);
		printf("Proc %d: %d\n", iam, data);
	}
	else{
		MPI_Recv(&send, 1, MPI_INT, iam - 1, tag, comm, &st);
		printf("Proc %d: %d\n", iam, data);
		MPI_Send(&send, 1, MPI_INT, iam + 1, tag, comm);
	}

	MPI_Finalize();
		
	return 0;
}
