#include <stdio.h>

double ave(double a, double b);

int main(void)
{
	double average;
	average = ave(12.3, 34.5);
	printf("%f\n", average);
	return 0;
}

double ave(double a, double b)
{
	return (a + b) / 2;
}

/*
23.400000
*/