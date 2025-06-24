#include <stdio.h>

int compare(char str1[], char str2[]);

int main(void)
{
	int result;
	char s1[100], s2[100];
	printf("１つ目の文字列は？：");
	scanf("%s", s1);
	printf("２つ目の文字列は？：");
	scanf("%s", s2);
	result = compare(s1, s2);
	printf("%d\n", result);
	return 0;
}

int compare(char str1[], char str2[])
{
	int i = 0;
	while (str1[i] != 0) {
		if (str1[i] == str2[i]) {
			break;
		} else {
			return 0;
		}
		i++;
	}
	return 1;
}

/*
１つ目の文字列は？：Medsys
２つ目の文字列は？：medsys
0
*/