#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <math.h>
#include "myhead.h"

#define N 6

void MPI_Comm_split2d(MPI_Comm comm, int iam, int q, MPI_Comm *row_comm, MPI_Comm *col_comm)
{
	int row_color = iam / q;
	MPI_Comm_split(comm, row_color, iam, row_comm);
	
	int col_color = iam % q;
	MPI_Comm_split(comm, col_color, iam, col_comm);
}

void matmul(int *A, int *B, int *C, int n)
{
	int row = 0, col = 0;
	for(int i = 0; i < n * n; i++){
		row = (int)(i / n) * n;
		col = i % n;
		for(int j = 0; j < n; j++)
			C[i] += A[row+j] * B[col + j * n];
	}
}

void align(int row_iam, int row_np, MPI_Comm row_comm,
	   int col_iam, int col_np, MPI_Comm col_comm,
	   int *tmp_A, int *tmp_B, int *buffer, int count,
	   MPI_Datatype datatype)
{
	// aligning
	int row_next, row_front;
	int col_next, col_front;
	row_next = (row_iam - col_iam + row_np) % row_np;
	row_front = (row_iam + col_iam) % row_np;
	col_next = (col_iam - row_iam + col_np) % col_np;
	col_front = (col_iam + row_iam) % col_np;

	MPI_Status st;
	int tag = 0;
	if(col_iam > 0){
		memcpy(buffer, tmp_A, sizeof(int)*count);
		MPI_Sendrecv(buffer, count, datatype, row_next, tag, tmp_A, count, datatype, row_front, tag, row_comm, &st);
	}
	if(row_iam > 0){
		memcpy(buffer, tmp_B, sizeof(int)*count);
		MPI_Sendrecv(buffer, count, datatype, col_next, tag, tmp_B, count, datatype, col_front, tag, col_comm, &st);
	}
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

	int A[N][N], B[N][N];
	for(int i = 0; i < N; i++)
		for(int j = 0; j < N; j++){
			A[i][j] = 0;
			B[i][j] = 0;
		}
	
	int p = sqrt(np);
	int subN = N / p;
	
	int tmp_A[subN * subN], tmp_B[subN * subN], tmp_C[subN*subN];
	int buffer[subN * subN];
	for(int i = 0; i < subN * subN; i++){tmp_A[i] = 0; tmp_B[i] = 0; tmp_C[i] = 0; buffer[i] = 0;}

	// matric initail 
	// send/recv original data 
	if(iam == 0){
		for(int i = 0; i < N; i++)
			for(int j = 0; j < N; j++){
				A[i][j] = i * N + j;
				B[i][j] = i * N + j;
			}
		for(int i = 1; i < np; i++){
			for(int j = 0; j < subN * subN; j++){
				tmp_A[j] = A[i/p*subN + j/subN][i%p*subN + j%subN];
				tmp_B[j] = B[i/p*subN + j/subN][i%p*subN + j%subN];
			}
			MPI_Send(tmp_A, subN*subN, MPI_INT, i, 0, comm);
			MPI_Send(tmp_B, subN*subN, MPI_INT, i, 99, comm);
		}
		for(int i = 0; i < subN * subN; i++){
			tmp_A[i] = A[i / subN][i % subN];
			tmp_B[i] = B[i / subN][i % subN];
		}
	}
	else{
		MPI_Recv(tmp_A, subN*subN, MPI_INT, 0, 0, comm, &st);
		MPI_Recv(tmp_B, subN*subN, MPI_INT, 0, 99, comm, &st);
	}

	// split comm 2d
	MPI_Comm row_comm, col_comm;
	MPI_Comm_split2d(comm, iam, p, &row_comm, &col_comm);

	int row_iam, col_iam, row_np, col_np;
	MPI_Comm_rank(row_comm, &row_iam);
	MPI_Comm_rank(col_comm, &col_iam);
	MPI_Comm_size(row_comm, &row_np);
	MPI_Comm_size(col_comm, &col_np);

	int row_next, col_next;
	int row_front, col_front;

	// alignment
	align(row_iam, row_np, row_comm, col_iam, col_np, col_comm, tmp_A, tmp_B, buffer, subN*subN, MPI_INT);
	
	// compute into tmp_C
	// move 1 proc per time
	for(int i = 0; i < p; i++){
		matmul(tmp_A, tmp_B, tmp_C, subN);
		row_next = (row_iam - 1 + row_np) % row_np;
		row_front = (row_iam + 1) % row_np;
		col_next = (col_iam - 1 + col_np) % col_np;
		col_front = (col_iam + 1) % col_np;
		memcpy(buffer, tmp_A, sizeof(tmp_A));
		MPI_Sendrecv(buffer, subN*subN, MPI_INT, row_next, tag, tmp_A, subN*subN, MPI_INT, row_front, tag, row_comm, &st);
		memcpy(buffer, tmp_B, sizeof(tmp_B));
		MPI_Sendrecv(buffer, subN*subN, MPI_INT, col_next, tag, tmp_B, subN*subN, MPI_INT, col_front, tag, col_comm, &st);
	}
	printf("Proc %d:", iam);
	VecPrint(tmp_C, subN*subN, sizeof(int));
	MPI_Finalize();

	return 0;	
}		
	

