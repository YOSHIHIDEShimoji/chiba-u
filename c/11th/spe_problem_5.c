#include <stdio.h>

void reverse(char str[]);
int count_n(char str[]);

int main(void)
{
	char s[100];
	int number;
	printf("文字列は？：");
	scanf("%s%*c", s);
	reverse(s);
}

void reverse(char str[])
{
	int m;
	m = count_n(str);
	char ss[m];
	for (int i = 0; i < m / 2; i++) {
		ss[m] = str[i];
	}
	printf("%s\n", ss);
	
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