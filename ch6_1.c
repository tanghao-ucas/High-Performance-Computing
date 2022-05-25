#include <stdio.h>
#include <math.h>
#include <complex.h>

#define PI 3.14159265358

void solve(int m, double complex *result)
{
	int total = pow(2, m);
	for(int i = 0; i < total; i++)
		result[i] = cos(2 * PI * i / total) + sin(2 * PI * i / total) * I;
}

int main()
{
	int m;
	printf("Input m value:");
	scanf("%d", &m);
	
	int count = pow(2, m);
	double complex result[count];

	solve(m, result);

	for(int i = 0; i < count; i++)
		printf("result[%d]: (%.2f) + i(%.2f)\n", i, creal(result[i]), cimag(result[i]));

	return 0;
}
	
