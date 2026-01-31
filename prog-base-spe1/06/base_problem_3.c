#include <stdio.h>

int main(void)
{
	int i, j;
	char c, c2[120] = {0};
	printf("input char:");
	scanf("%c", &c);

	if (32 <= c && c <= 117) {
		for (i = 0; i < 5; i++) {
			for (j = 0; j < 10; j++) {
				c2[j] = c + j;
			}
		printf("%s", c2);
		}
	}	
	return 0;
	
}

/*
input char:G
GHIJKLMNOPGHIJKLMNOPGHIJKLMNOPGHIJKLMNOPGHIJKLMNOP
*/
