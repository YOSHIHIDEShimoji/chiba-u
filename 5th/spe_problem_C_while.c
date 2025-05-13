#include <stdio.h>

int main(void)
{
	int i = 2, sum = 0;
	while (i <= 100) {
		sum += i;
		i += 2;
	}
	printf("sum = %d\n", sum);
	return 0;
}