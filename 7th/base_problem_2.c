#include <stdio.h>
void prin1(void);

int main(void)
{
	prin1(); prin1(); prin1();
	prin1(); prin1(); prin1();
	return 0;
}

void prin1(void)
{
	static int i = 1;
	printf("in function prin1(): %d\n", i);
	i++;
}