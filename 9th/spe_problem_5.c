#include <stdio.h>

int main(void)
{
	int x, array[3];
	int *ptr = array;

	printf("Address of x: %p\n", &x);

	for (int i = 0; i < 3; i++) {
		printf("Address of array[%d]: %p\n", i, ptr);
		ptr++;
	}
}
