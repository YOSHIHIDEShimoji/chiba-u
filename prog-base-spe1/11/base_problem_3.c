#include <stdio.h>

int main(void)
{
	int a, b, wariai;
	printf("２つの整数値を入力して下さい.\n");
	printf("整数a: ");	scanf("%d", &a);
	printf("整数b: ");	scanf("%d", &b);
	wariai = (int)(((double)a / (double)b) * 100);
	printf("aはbの%d%です.\n", wariai);
	return 0;
}

/*
２つの整数値を入力して下さい.
整数a: 12
整数b: 23
aはbの52%です.
*/