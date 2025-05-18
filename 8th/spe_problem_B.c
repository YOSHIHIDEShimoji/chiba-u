#include <stdio.h>

void sum_char_code(char ss[]);

int main(void)
{
	char name[] = {0};
	printf("Input Name = ");
	scanf("%s", &name);
	sum_char_code(name);
	return 0;
}

void sum_char_code(char ss[])
{
	int i, sum = 0;
	for (i = 0; ss[i] != '\0'; i++) {
		sum += ss[i];
	}
	printf("%s -> %d\n", ss, sum);
}