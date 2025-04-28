# include <stdio.h>

int main(void)
{
	char a = 127;
	unsigned char b = 200;
	short c = 300;
	int d = 123456;
	float e = 1234.5;
	long f = 12345678901234;

	printf("%d\n", a);
	printf("%u\n", b);
	printf("%d\n", c);
	printf("%d\n", d);
	printf("%f\n", e);
	printf("%ld\n", f);

	return 0;
}

/*
127
200
300
123456
1234.500000
12345678901234
*/
