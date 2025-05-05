#include <stdio.h>

int main(void)
{
	int i, n;
	for (n = 2; n <= 10000; n++) {
		i = 1;
		while (++i < n) {
			if (n % i == 0) {	
				break;
			}	
		}
		if (i == n) {
			printf("%d ", n);
		}
	}
	return 0;
}

