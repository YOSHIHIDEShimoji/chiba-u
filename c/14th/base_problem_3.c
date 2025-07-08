#include <stdio.h>

double some(double a, double b, double c);

int main(void)
{
	int score[3][3] = {{50, 60, 70},
						{60, 70, 50},
						{100, 80, 90}};

	printf("Aさんの合計点 = %f\n", some(score[0][0], score[0][1], score[0][2]));
	printf("Bさんの合計点 = %f\n", some(score[1][0], score[1][1], score[1][2]));
	printf("Cさんの合計点 = %f\n", some(score[2][0], score[2][1], score[2][2]));

	return 0;
}

double some(double a, double b, double c)
{
	return a + b + c;
}

/*
Aさんの合計点 = 180.000000
Bさんの合計点 = 180.000000
Cさんの合計点 = 270.000000
*/