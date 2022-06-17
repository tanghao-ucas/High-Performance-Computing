#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myhead.h"

void matradd(int an, int *A, int bn, int *B, int m, int n, int *C)
{
	for(int i = 0; i < m; i++)
		for(int j = 0; j < n; j++)
			C[i * n + j] = A[i * an + j] + B[i * bn + j];
}

int main()
{
	int A[25], B[36], C[16];
	for(int i = 0; i < 36; i++){
		if(i < 25) A[i] = i * 2;
		if(i < 16) C[i] = 0;
		B[i] = i + 10;
	}

	matradd(4, 4, 5, A, 6, B, C);

	printf("A:\n");
	for(int i = 0; i < 5; i++)
		VecPrint(A + i * 5, 5, sizeof(int));
	printf("B:\n");
	for(int i = 0; i < 6; i++)
		VecPrint(B + i * 6, 6, sizeof(int));
	printf("C:\n");
	for(int i = 0; i < 4; i++)
		VecPrint(C + i * 4, 4, sizeof(int));
}	
