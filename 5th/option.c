#include <stdio.h>

int main(void)
{
	int i;
	for (i = 2000; i <= 2200; i++) {
		if (i % 4 == 0) {
			if (i % 100 == 0) {
				if (i % 400 == 0) {
					printf("%d\n", i);
				}
			} else {
				printf("%d\n", i);
			}
		}
    }
}