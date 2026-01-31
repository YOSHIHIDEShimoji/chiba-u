#include <stdio.h>

int main(void)
{
	double cm;
	printf("長さをcmで入力して下さい: ");
	scanf("%lf", &cm);
	printf("それは%fインチです.\n", cm / 2.54);
	return 0;
}

/*
長さをcmで入力して下さい: 4
それは1.574803インチです.
*/