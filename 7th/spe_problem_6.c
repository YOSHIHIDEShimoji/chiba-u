#include <stdio.h>
void min_max_char(char a);

int main(void)
{
	char x, i;
	int xx;
	for (i = 0; i < 5; i++) {
		printf("Input = "); scanf("%d", &xx);
		x = (char)xx;
		min_max_char(x);
	}
}

void min_max_char(char a)
{
	static char min, max, tmp = 0;
	int minmin, maxmax;

	if (tmp == 0) {
		min = a;
		max = a;
		tmp++;
	}

	if (a < min) {
		min = a;
	} else if (a > max) {
		max = a;
	}

	minmin = (int)min; maxmax = (int)max;
	printf("min = %d, max = %d\n", minmin, maxmax);
}

/*
Input = 10
min = 10, max = 10
Input = 6
min = 6, max = 10
Input = 78
min = 6, max = 78
Input = -9
min = -9, max = 78
Input = 55
min = -9, max = 78
*/
