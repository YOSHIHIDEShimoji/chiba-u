#include <stdio.h>

int main(void)
{
	int x = 1, y = 23, temp;
	int *ptr1 = &x, *ptr2 = &y;
	printf("x = %d\ty = %d\n", x, y);
	
	temp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = temp;
	
	printf("x = %d\ty = %d\n", x, y);
	
	return 0;
}

/*x = 1	y = 23
x = 23	y = 1
*/