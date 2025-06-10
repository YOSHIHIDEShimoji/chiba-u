#include <stdio.h>

int main(void)
{
	int x;
	printf("アドレスは%pです。値は%dです。\n", &x, x);
	return 0;
}