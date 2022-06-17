#include <stdio.h>
#include "myhead.h"

void bit_reverse(int n, int *arr)
{
	if(n < 2){
		arr[0] = 0;
		return;
	}

	int n2 = 2;
	arr[0] = 0; arr[1] = 1;

	while(n2 < n){
		for(int i = 0; i < n2; i++){
			arr[i] *= 2;
			arr[i + n2] = arr[i] + 1;
		}
		n2 *= 2;
	}
}

void array_reverse(int n, int *arr)
{
	int place[n];
	bit_reverse(n, place);
	
	/*
	int tmp[n];
	for(int i = 0; i < n; i++) tmp[i] = arr[i];
	for(int i = 0; i < n; i++) arr[i] = tmp[place[i]];	
	*/
	for(int i = 0; i < n; i++)
		if(i < place[i]) swap(&arr[i], &arr[place[i]], sizeof(int));

}

int main()
{
	int arr[16];
	for(int i = 0; i < 16; i++) arr[i] = 10 + i;
	array_reverse(16, arr);
	VecPrint(arr, 16, sizeof(int));
	return 0;
}
