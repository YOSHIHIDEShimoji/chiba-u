#include <stdio.h>

int main(void)
{
	int i = 0;
	char c = 'A';
	while (c <= 'Z') {
		printf("%c", c++);
		if (++i % 10 == 0) {
			printf("\n");
		}
	}
}
