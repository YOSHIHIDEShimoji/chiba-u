#include <stdio.h>

int main(void)
{
	int i, sum = 0;
	for (i = 2; i <= 100; i += 2) {
		sum += i;
	}
	printf("sum = %d\n", sum);
	return 0;
}