#include <stdio.h>

#ifndef MYHEAD

#define MYHEAD

#define T_CHAR 1
#define T_INT 2
#define T_FLOAT 3
#define T_DOUBLE 4


void VecPrint(void *vec, int n, int type);

void MatrPrint(void **Matr, int row, int col, int type);

#endif
