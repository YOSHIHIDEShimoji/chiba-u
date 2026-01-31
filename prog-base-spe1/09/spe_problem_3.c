#include <stdio.h>

int main(void)
{
	int num1 = 1, num2 = 2;
	int *ptr1 = &num1, *ptr2 = &num2;
	printf("num1 = %d\n", *ptr1);
	printf("num2 = %d\n", *ptr2);
	ptr1 = ptr2;
	*ptr1 = 5;
	printf("num1 = %d\n", num1);
	printf("num2 = %d\n", num2);

	return 0;
}

/*
num1 = 1
num2 = 2
num1 = 1
num2 = 5
*/