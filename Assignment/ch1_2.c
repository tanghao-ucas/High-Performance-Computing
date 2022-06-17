#include <stdio.h>

int poly_sum(int *cf, int x, int n)
{
	int result = 0;
	int tmp_x = 1;
	for(int i = 0; i <= n; i++){
		result += (tmp_x * cf[i]);
		tmp_x *= x;
	}
	return result;
}

int main()
{
	int a[5] = {1, 1, 1, 1, 1};
	int x = 5;
	int n = 4;

	printf("5^4 = %d\n", poly_sum(a, x, n));

}
