#include <stdio.h>

int main(void)
{
	int i = 2, sum = 0;
	do {
		sum += i;
		i += 2;
	} while (i <= 100);
	printf("sum = %d\n", sum);
	return 0;
}