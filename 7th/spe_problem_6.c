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