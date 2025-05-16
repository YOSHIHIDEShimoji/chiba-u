#include <stdio.h>
int min_int(int a, int b);

int main(void)
{
	int a, b, m;
	printf("Enter two integer values.\n");
	printf("integer a:");
	scanf("%d", &a);
	printf("integer b:");
	scanf("%d", &b);
	m = min_int(a, b);
	printf("The smaller value is %d\n", m);
	return 0;
}

int min_int(int a, int b)
{
	int min;
	if (a - b >= 0) {
		min = b;
	} else {
		min = a;
	}
	return min;
}