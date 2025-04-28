# include <stdio.h>

int main(void)
{
	char a[8] = {1, 2, 4, 8, 16, 32, 64, 128};
	
	int i;
	int x = 0;
	for(i = 0; i <= 7; i++){
		printf("a[%d] = %d\n", i, a[i]);
		x = x + a[i];
	}
	printf("The total is %d\n", x);

	return 0;
}

/*
a[0] = 1
a[1] = 2
a[2] = 4
a[3] = 8
a[4] = 16
a[5] = 32
a[6] = 64
a[7] = -128
The total is -1
*/
