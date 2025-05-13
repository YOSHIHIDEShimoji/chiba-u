#include <stdio.h>

int main(void)
{
int i;
	for (i = 2000; i <= 2200; i++) {
		if (i % 400 == 0) {
			printf("%d\n", i);
		}
		else if (i % 100 == 0) {
		}
		else if (i % 4 == 0) {
			printf("%d\n", i);
		}
	}
	return 0;
}