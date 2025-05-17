#include <stdio.h>
void prin0(int a);

int main(void)
{
	int i = 1;
	prin0(i);
	i++;
	prin0(i);
	return 0;
}

void prin0(int a)
{
	printf("Input no. = %d\n", a);
}