#include <stdio.h>

int main(void)
{
	int n, i = 0;
	printf("Enter an integer: ");
	scanf("%d", &n);
	printf("The divisor of %d is ", n);
	while (++i <= n) {
		if (n % i == 0) {
			printf("%d ", i);
		}
	}
	return 0;
}

/*
The divisor of 9367 is 1 17 19 29 323 493 551 9367
*/
