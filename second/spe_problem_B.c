#include <stdio.h>

int main()
{
	char c = 127;
	printf("c = %d\n", c);

	c = c + 1;
	printf("c = %d\n", c);
	
	return 0;
}

/*
c is a char type, and the range of values that can be represented by the char type is -128 to 127. Adding 1 to 127 exceeds the range of values that can be represented.
Translated by DeepL
*/
