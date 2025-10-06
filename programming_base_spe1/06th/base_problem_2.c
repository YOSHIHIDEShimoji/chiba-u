#include <stdio.h>

int main(void)
{
	int dt;
	printf("input integer (32 - 126):");
	while (scanf("%d", &dt) != EOF) {
		if (32 <= dt && dt <= 126) {
			printf("%c\n", dt);
			printf("%o\n", dt);		
			printf("%d\n", dt);
			printf("%10.7f\n", dt / 3.0);
		}
	}	
}

/*
input integer (32 - 126):79
O
117
79
26.3333333
*/
