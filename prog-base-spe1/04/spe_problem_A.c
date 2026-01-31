#include <stdio.h>

int main(void)
{
	int i = 1;
	while (i <= 30){
		printf("%3d", i++);
		if ((i - 1) % 10 == 0){
			printf("\n");
		}
	}
	
	return 0;

}
