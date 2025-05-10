#include <stdio.h>

int main(void)
{
	int a, b;
	char c;
	double e;
	a = b = 2;
	b += 2;		/* 4 */
	c = 95;
	b++;		/* 5 */
	b += c;		/* 100 */
	printf("%d\n", b++);
	printf("%d\n", b);
	e = b;
	printf("%f\n", e);
	
	return 0;
}

/*
100
101
101.000000
*/
