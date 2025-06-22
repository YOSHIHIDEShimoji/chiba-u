#include <stdio.h>

int main(void)
{
	int x, i;
	printf("整数値を入力して下さい: ");
	scanf("%d", &x);
	for (i = 1; i <= x; i++) {
		printf("%d ", i);
	}
	printf("\n");
	return 0;
}