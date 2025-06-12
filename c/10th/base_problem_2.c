#include <stdio.h>

void hyouji(int *a, int b);

int main(void)
{
	int x[] = {2, 8, 5, 2, 5};
	hyouji(x, 5);
	return 0;
}

void hyouji(int *a, int b)
{
	int wa = 0;
	for (int i = 0; i < b; i++) {
		wa += a[i];
	}
	printf("総和 = %d\n", wa);
}