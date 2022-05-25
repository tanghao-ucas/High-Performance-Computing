#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "myhead.h"
#include <stddef.h>

#define INT_NUM 3
#define FLOAT_NUM 2
#define CHAR_NUM 5
#define STRUCT_NUM 10

typedef struct data{
	int m[INT_NUM];
	float a[FLOAT_NUM];
	char c[CHAR_NUM];
}Data;

void StructPrint(Data *s)
{
	VecPrint(s->m, INT_NUM, sizeof(int));
	for(int j = 0;j < FLOAT_NUM; j++) printf("%f ", s->a[j]);
	printf("\n");
	for(int i = 0; i < CHAR_NUM; i++) printf("%c", s->c[i]);
	printf("\n");
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

	// create & initial structs
	Data x[STRUCT_NUM];
	for(int i = 0; i < STRUCT_NUM; i++){
		for(int j = 0; j < INT_NUM; j++) x[i].m[j] = 0;
		for(int j = 0; j < FLOAT_NUM; j++) x[i].a[j] = 0.0;
		for(int j = 0; j < CHAR_NUM; j++) x[i].c[j] = 'a';
	}
	
	// create mpi struct datatype for transmit
	int count = 3;
	int blocklen[3] = {INT_NUM, FLOAT_NUM, CHAR_NUM};
	MPI_Aint disp[3] = {offsetof(Data, m), offsetof(Data, a), offsetof(Data, c)};
	MPI_Datatype oldtype[3] = {MPI_INT, MPI_FLOAT, MPI_CHAR};
	MPI_Datatype newtype;

	MPI_Type_struct(count, blocklen, disp, oldtype, &newtype);
	MPI_Type_commit(&newtype);

	int root = 0, dest = 1;
	int tag = 0;
	if(iam == 0){
		for(int i = 0; i < STRUCT_NUM; i++){
			for(int j = 0; j < INT_NUM; j++) x[i].m[j] = i*10 + j;
			for(int j = 0; j < FLOAT_NUM; j++) x[i].a[j] = i + 0.3 * j;
			for(int j = 0; j < CHAR_NUM; j++) x[i].c[j] = 'a' + i + 1;
		}
		
		MPI_Send(x, STRUCT_NUM, newtype, dest, tag, comm);
	}
	else{
		MPI_Recv(x, STRUCT_NUM, newtype, root, tag, comm, &st);
		for(int i = 0; i < STRUCT_NUM; i++){
			printf("In x[%d]:\n", i);
			StructPrint(&x[i]);
		}
	}
	
	MPI_Finalize();

	return 0;	
}	





