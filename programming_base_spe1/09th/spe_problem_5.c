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

/*
Address of x: 0x7ffddb08a7d0
Address of array[0]: 0x7ffddb08a7c4
Address of array[1]: 0x7ffddb08a7c8
Address of array[2]: 0x7ffddb08a7cc
*/