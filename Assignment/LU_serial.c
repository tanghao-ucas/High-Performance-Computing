#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myhead.h"

void swap(double *a, double *b, int size)
{
	double tmp[size];
	memcpy(tmp, a, sizeof(double)*size);
	memcpy(a, b, sizeof(double)*size);
	memcpy(b, tmp, sizeof(double)*size);
}

int main()
{
	int n;
	FILE *file = fopen("input3.txt", "r");
	fscanf(file, "%d", &n);
	double A[n][n+1];
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n + 1; j++)
			fscanf(file, "%lf", &A[i][j]);
	}

	for(int i = 0; i < n; i++)
		VecPrint(A[i], n + 1, T_DOUBLE);

	double coef = 0;
	for(int j = 0; j < n; j++){
		int maxi = j;
		for(int i = j + 1; i < n; i++)
		       if(A[i][j] > A[maxi][j]) maxi = i;
		if(maxi != j) swap(A[j], A[maxi], n + 1);
		
		if(A[j][j] == 0){ printf("A is singular!\n"); return 0; }

		for(int i = 0; i < n; i++){
			if(i != j){
				coef = A[i][j] / A[j][j];
				printf("for line %d, coefficient = %f\n", i, coef);
				for(int k = j; k < n + 1; k++)
					A[i][k] -= (coef * A[j][k]);
			}
		}
		// print the cuurent matrix
		printf("column %d:\n", j);
		for(int i = 0; i < n; i++)
			VecPrint(A[i], n+1, T_DOUBLE);
	}
	return 0;
}

