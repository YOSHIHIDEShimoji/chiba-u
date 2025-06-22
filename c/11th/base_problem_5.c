#include <stdio.h>

int main(void)
{
	int x;
	printf("整数値を入力して下さい: ");
	scanf("%d", &x);
	if (x % 10) {
		printf("%dは10の倍数ではありません.\n", x);
	} else {
		printf("%dは10の倍数です.\n", x);
	}
	return 0;
}