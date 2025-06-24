#include <stdio.h>

int count_n(char str[]);

int main(void)
{
	char s[100];
	int number;
	printf("文字列は？：");
	scanf("%s", s);
	number = count_n(s);
	printf("文字数は%d個\n", number);
	return 0;
}

int count_n(char str[])
{
	int n = 0, i = 0;
	while (str[i] != 0) {
		n++;
		i++;
	}
	return n;
}