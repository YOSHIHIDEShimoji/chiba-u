#include <stdio.h>

int main(void)
{
	int a[3] = {1, 2, 3};
	
	int k = 0;
	while (k < 3) {
		printf("%d ", a[k]);
		k++;
	}

	printf("\n");

	for (int i = 0; i < 3; i++) {
		printf("%d ", a[i]);
	}

	return 0;
}
