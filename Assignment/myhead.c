#include <stdio.h>
#include <stdlib.h>
#include "myhead.h"

void VecPrint(void *vec, int n, int type)
{
	if(type == T_CHAR)
		for(int i = 0; i < n; ++i)
			printf("%c ", *(char*)(vec + i * sizeof(char)));
	if(type == T_INT)
		for(int i = 0; i < n; ++i)
			printf("%d ", *(int*)(vec + i * sizeof(int)));
	if(type == T_FLOAT)
		for(int i = 0; i < n; ++i)
			printf("%f ", *(float*)(vec + i * sizeof(float)));
	if(type == T_DOUBLE)
		for(int i = 0; i < n; ++i)
			printf("%lf ", *(double*)(vec + i * sizeof(double)));
	printf("\n");
}

void MatrPrint(void **matr, int row, int col, int type)
{
	for(int i = 0; i < row; ++i)
		VecPrint(matr[i], col, type);	
}
