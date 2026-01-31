#include <stdio.h>

double ave(double a, double b, double c);

int main(void)
{
	int score[3][3] = {{50, 60, 70},
					   {60, 70, 50},
					   {100, 80, 90}};

	printf("算数の平均点 = %f\n", ave(score[0][0], score[1][0], score[2][0]));
	printf("算数の平均点 = %f\n", ave(score[0][1], score[1][1], score[2][1]));
	printf("算数の平均点 = %f\n", ave(score[0][2], score[1][2], score[2][2]));

	return 0;
}

double ave(double a, double b, double c)
{
	return (a + b + c) / 3;
}

/*
算数の平均点 = 70.000000
算数の平均点 = 70.000000
算数の平均点 = 70.000000
*/