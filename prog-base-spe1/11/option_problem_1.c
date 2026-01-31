#include <stdio.h>

void narabekae(int array[], int n);
int min_positioin(int array[], int start, int end);

int main(void)
{
	int a[10] = {3, 6, 9, 0, 4, 2, 5, 1, 8, 7};
	narabekae(a, 10);
	for (int i = 0; i < 10; i++) {
		printf("%2d", a[i]);
	}
	printf("\n");
	return 0;
}

void narabekae(int array[], int n)
{
	for (int i = 0; i < n; i++) {
		int m = min_positioin(array, i, n - 1);
		int temp = array[i];
		array[i] = array[m];
		array[m] = temp;
	}
}

int min_positioin(int array[], int start, int end)
{
	int min = start;
	for (int j = start + 1; j <= end; j++) {
		if (array[j] < array[min]) {
			min = j;
		}
	}
	return min;
}