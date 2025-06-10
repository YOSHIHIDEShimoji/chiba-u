#include <stdio.h>

int main()
{
	short int a = 32767;
	int b = 2147483647;
	long int c = 9223372036854775807L;
	unsigned int d = 4294967295U;

	printf("a = %d\n", a);
	printf("b = %d\n", b);
	printf("c = %ld\n", c);
	printf("d = %u\n", d);

	return 0;
}

/*
a = 32767
b = 2147483647
c = 9223372036854775807
d = 4294967295
*/
