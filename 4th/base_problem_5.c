#include <stdio.h>

int main(void)
{
	double d1;
	scanf("%lf", &d1);
	
	int i1;
	i1 = (int)(d1 + 0.5);
	printf("The rounded result of d1 = %f is %d.\n", d1, i1);
	return 0;
}

/*
Translated by DeepL
*/
