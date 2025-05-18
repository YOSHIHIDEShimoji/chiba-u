#include <stdio.h>

void char_code(char ss[]);

int main(void)
{
	char name[] = {0};
	printf("Input Name = ");
	scanf("%s", &name);
	char_code(name);
	return 0;
}

void char_code(char ss[])
{
	int i;
	for (i = 0; 1; i++) {
		printf("%d\n", ss[i]);
		if (ss[i] == '\0') {
			break;
		}
	}
}