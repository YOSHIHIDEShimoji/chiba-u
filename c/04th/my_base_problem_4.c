#include <stdio.h>

int main(void)
{
	int a, b;
	char c;
	double e;
	a = b = 2;
	b += 2;
	printf("No.1\n");
	printf("b = %d\n", b);
	c = 95;
	b += c;
	b++;
	printf("No.2\n");
	printf("b = %d\n", b);
	b += c;
	printf("No.3\n");
	printf("b = %d\n", b);

	printf("b = %d\n", b++);
	printf("b = %d\n", b);
	e = b;
	printf("e = %f\n", e);
	
	return 0;
}
