#include <stdio.h>
int main()
{
	float x1 = 123456789.56789;
	float x2 = 1.001;
	
	double y1 = 123456789.56789;
	double y2 = 1.001;

	printf("x1 = %f\n", x1);
	printf("y1 = %f\n", y1);
	
	printf("x1 - x2 = %f\n", x1 - x2);
	printf("y1 - y2 = %f\n", y1 - y2);

	return 0;
}
