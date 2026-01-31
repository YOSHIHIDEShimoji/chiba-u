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

/*
in function prin1(): 1
in function prin1(): 2
in function prin1(): 3
in function prin1(): 4
in function prin1(): 5
in function prin1(): 6
*/
