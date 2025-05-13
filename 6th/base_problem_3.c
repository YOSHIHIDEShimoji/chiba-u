#include <stdio.h>

int main(void)
{
	int i, j;
	char c, c2[120] = {0};
	printf("input char:");
	scanf("%c", &c);

	if (32 <= c && c <= 126) {
		for (j =0; j < 5; j++) {
			for (i = 0; i < 10; i++) {
				c2[i] = c + i;
			}
		printf("%s", c2);}
	}	
	return 0;
	
}