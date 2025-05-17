#include <stdio.h>
int pw(int a, int b);

int main(void)
{
	int x, y, z;
	printf("x = "); scanf("%d", &x);
	printf("y = "); scanf("%d", &y);
	z = pw(x, y);
	printf("%d ^ %d = %d\n", x, y, z);
	return 0;
}

int pw(int a, int b)
{
	int i, value = 1;
	for (i = 0; i < b; i++) {
		value *= a;
	}
	return value;
}
