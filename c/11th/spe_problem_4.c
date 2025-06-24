#include <stdio.h>

int count_c(char str[], char c);

int main(void)
{
	char s[100], c[1];
	int number;
	printf("文字列は？：");
	scanf("%s%*c", s);
	printf("探す文字は？：");
	scanf("%s%*c", c);
	number = count_c(s, c[1]);
	printf("文字%cは%sの中に%dこあります\n", c[1], s, number);
	return 0;
}

int count_c(char str[], char c)
{
	int n = 0, i = 0;
	while (str[i] != 0) {
		if (str[i] == c) {
			n++;
		}
		i++;
	}
	return n;
}