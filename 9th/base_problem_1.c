#include <stdio.h>

int main(void)
{
	int x;
	printf("The address is %p. The value is %d.\n", &x, x);
	return 0;
}

/*
The address is 0x7fffba7702ec. The value is 30788.
*/