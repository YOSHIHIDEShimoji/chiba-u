#include <stdio.h>

int main(void)
{
	int a[] = {5, 4, 3, 2, 1};

	for (int i = 0; i < 5; i++) {
		printf("a[%d] = %d\n", i, a[i]);
	}
	return 0;
}

/*
a[0] = 5
a[1] = 4
a[2] = 3
a[3] = 2
a[4] = 1
*/