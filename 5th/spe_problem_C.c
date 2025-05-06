#include <stdio.h>

int main(void)
{
	int i, sum = 0;
	for (i = 2; i <= 100; i += 2) {
		sum += i;
	}
	printf("sum = %d\n", sum);

	i = 2; sum = 0;
	while (i <= 100) {
		sum += i;
		i += 2;
	}
	printf("sum = %d\n", sum);
	
	i = 2; sum = 0;
	do {
		sum += i;
		i += 2;
	} while (i <= 100);
	printf("sum = %d\n", sum);
	
	return 0;
}