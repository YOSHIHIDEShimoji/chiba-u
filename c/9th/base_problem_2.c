#include <stdio.h>

int main(void)
{
	double x[3];
	double *ptr = x;
	for (int i = 0; i < 3; i++) {
		printf("The address of x[%d]is %p. The value is %f.\n", i, ptr, *ptr);
		*ptr = 1.23;
		ptr++;
	}

	/*
	printf("\n");
	ptr = x;
	for(int j = 0; j < 3; j++) {
		printf("The address of x[%d]is %p. The value is %f.\n", i, ptr, *ptr);
		*ptr = 1.23;
		ptr++;
	}
	*/
	return 0;
}

/*
The address of x[0]is 0x7ffe58743f50. The value is 0.000000.
The address of x[1]is 0x7ffe58743f58. The value is 0.000000.
The address of x[2]is 0x7ffe58743f60. The value is 0.000000.
*/