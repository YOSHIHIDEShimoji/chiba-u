#include <stdio.h>
int func(int a, int b);

int main(void)
{
	int x, y, z;
	printf("x = "); scanf("%d", &x);
	printf("y = "); scanf("%d", &y);
	z = func(x, y);
	printf("The sum of between %d and %d is %d\n", x, y, z);
	return 0;
}

int func(int a, int b)
{
	if (a == b) {
		return b;
	} else {
		int value;
		value = a + func(a + 1, b);
		return value;
	}
}