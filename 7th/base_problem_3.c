#include <stdio.h>
int pw3(int i);

int main(void)
{
	int x, y;
	printf("x = ");
	scanf("%d", &x);
	y = pw3(x);
	printf("the third power of %d = %d\n", x, y);
}

int pw3(int i)
{
	return i * i * i;
}