#include <stdio.h>

void rev_array(int array[], int len);

int main(void)
{
	int n;
	printf("Enter the number of elements in array v: ");
	scanf("%d", &n);
	
	int v[n];
	for (int i = 0; i < n; i++) {
		printf("Enter v[%d]: ", i);
		scanf("%d", &v[i]);
	}

	rev_array(v, n);

	for (int j = 0; j < n; j++) {
		printf("v[%d]: %d\n", j, v[j]);
	}
}

void rev_array(int array[], int len)
{
	for (int i = 0; i < len / 2; i++) {
		int tmp = array[i];
		array[i] = array[len - i - 1];
		array[len - i - 1] = tmp;
	}
}