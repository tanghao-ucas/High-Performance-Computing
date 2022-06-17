#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <math.h>
#include "myhead.h"

#define MAX 10

void gauss_seidel(
	double *tmp_A, 
	double tmp_B, 
	double *x, 
	double *s, 
	int n, 
	int iam, 
	MPI_Comm comm)
{
	int tag = 0;
	MPI_Status st;
	double val = 0.0;
	if(iam == 0){
		*x = (tmp_B - (*s)) / tmp_A[iam];
		*s = 0;
		for(int i = 1; i < n; i++){
			val = tmp_A[i] * (*x);
			MPI_Send(&val, 1, MPI_DOUBLE, i, tag, comm);
		}
		for(int i = 1; i < n; i++){
			MPI_Recv(&val, 1, MPI_DOUBLE, i, tag, comm, &st);
			(*s) += val;
		}
	}
	else{
		for(int i = 0; i < iam; i++){
			MPI_Recv(&val, 1, MPI_DOUBLE, i, tag, comm, &st);
			(*s) += val;
		}
		*x = (tmp_B - (*s)) / tmp_A[iam];
		*s = 0;
		for(int i = iam + 1; i < n; i++){
			val = tmp_A[i] * (*x);
			MPI_Send(&val, 1, MPI_DOUBLE, i, tag, comm);
		}
		for(int i = 0; i < iam; i++){
			val = tmp_A[i] * (*x);
			MPI_Send(&val, 1, MPI_DOUBLE, i, tag, comm);
		}
		for(int i = iam + 1; i < n; i++){
			MPI_Recv(&val, 1, MPI_DOUBLE, i, tag, comm, &st);
			(*s) += val;
		}
	}
}

void gauss_seidel_iter(
	double *tmp_A,
	double tmp_B,
	double *x,
	double *s,
	int n,
	double delta,
	int iam,
	int np,
	MPI_Comm comm)
{
	int tag = 99;
	MPI_Status st;

	int flag = 0;
	double originx = *x;
	double kx = 0;
	double bias1 = 0, bias2 = 0;
	double err1 = 0, err2 = 0;

	do{
		kx = *x;
		gauss_seidel(tmp_A, tmp_B, x, s, n, iam, comm);
		bias1 = pow(((*x) - kx), 2);
		bias2 = pow(((*x) - originx), 2);
		MPI_Reduce(&bias1, &err1, 1, MPI_DOUBLE, MPI_SUM, 0, comm);
		MPI_Reduce(&bias2, &err2, 1, MPI_DOUBLE, MPI_SUM, 0, comm);
		if(iam == 0){
			printf("err1: %lf,  err2: %lf\n", err1, err2);
			if(sqrt(err1) < sqrt(err2) * delta) flag = 1;
			for(int i = 1; i < np; i++)
				MPI_Send(&flag, 1, MPI_INT, i, tag, comm);
		}
		else
			MPI_Recv(&flag, 1, MPI_INT, 0, tag, comm, &st);
	}while(!flag);
}

int main()
{
	int iam, np;
	MPI_Comm comm;
	MPI_Status st;
	MPI_Request req;

	MPI_Init(0, 0);
	MPI_Comm_dup(MPI_COMM_WORLD, &comm);
	MPI_Comm_rank(comm, &iam);
	MPI_Comm_size(comm, &np);

	int n;
	double s = 0, x = 0;
	double A[MAX][MAX], B[MAX];
	for(int i = 0; i < MAX; i++){
		for(int j = 0; j < MAX; j++)
			A[i][j] = 0;
		B[i] = 0;
	}

	if(iam == 0){
		FILE *file = fopen("input5.txt", "r");
		fscanf(file, "%d", &n);

		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++)
				fscanf(file, "%lf", &A[j][i]);
			fscanf(file, "%lf", &B[i]);
		}
	}
	MPI_Bcast(&n, 1, MPI_INT, 0, comm);
	double tmp_A[n], tmp_B;

	int tag = 0;
	if(iam == 0){
		for(int i = 1; i < np; i++){
			MPI_Send(A[i], n, MPI_DOUBLE, i, tag, comm);
			MPI_Send(&B[i], 1, MPI_DOUBLE, i, tag, comm);
		}
		memcpy(tmp_A, A[0], sizeof(double)*n);
		tmp_B = B[0];
	}
	else{
		MPI_Recv(tmp_A, n, MPI_DOUBLE, 0, tag, comm, &st);
		MPI_Recv(&tmp_B, 1, MPI_DOUBLE, 0, tag, comm, &st);
	}

	double delta = 0.0001;
	gauss_seidel_iter(tmp_A, tmp_B, &x, &s, n, delta, iam, np, comm);
	
	
	double result[n];
	MPI_Gather(&x, 1, MPI_DOUBLE, result, 1, MPI_DOUBLE, 0, comm);

	if(iam == 0)
		VecPrint(result, n, T_DOUBLE);
		
	MPI_Finalize();

	return 0;
}







