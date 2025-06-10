#include <stdio.h>

int main(void)
{
	double x[3];
	double *ptr = x;
	for (int i = 0; i < 3; i++) {
		printf("x[%d]のアドレスは%pです。値は%fです。\n", i, ptr, *ptr);
		*ptr = 1.23;
		ptr++;
	}

	/*
	printf("\n");
	ptr = x;
	for(int j = 0; j < 3; j++) {
		printf("x[%d]のアドレスは%pです。値は%fです。\n", j, ptr, *ptr);
		*ptr = 1.23;
		ptr++;
	}
	*/
	return 0;
}