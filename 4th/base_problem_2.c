#include <stdio.h>

int main(void)
{
	int a = 1, b = 0;
	
	/*
	((a = b) = 2 + 1);
	printf("No.1\n");
	printf("a = %d\n", a);
	printf("b = %d\n", b);
	*/
	
	a = (b = 2) + 1;
	printf("No.2\n");
	printf("a = %d\n", a);
	printf("b = %d\n", b);

	a = b = 2 + 1;
	printf("No.3\n");
	printf("a = %d\n", a);
	printf("b = %d\n", b);

	return 0;
}

/*
No.1 is an error.
*/
