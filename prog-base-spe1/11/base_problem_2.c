#include <stdio.h>

int main(void)
{
	int a, b, seki;
	printf("２つの整数値を入力して下さい.\n");
	printf("整数a: ");	scanf("%d", &a);
	printf("整数b: ");	scanf("%d", &b);
	seki = a * b;
	printf("２つの整数値の積の値は%dです.\n", seki);
	return 0;
}

/*
２つの整数値を入力して下さい.
整数a: 23
整数b: 2
２つの整数値の積の値は46です.
*/