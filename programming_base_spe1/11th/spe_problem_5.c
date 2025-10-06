#include <stdio.h>

void reverse(char str[]);
int count_n(char str[]);

int main(void)
{
	char s[100];
	printf("文字列は？：");
	scanf("%s%*c", s);
	reverse(s);
	return 0;
}

void reverse(char str[])
{
	int m = count_n(str);
	char ss[100];
	for (int i = 0; i < m; i++) {
		ss[i] = str[m - 1 - i];
	}
	ss[m] = '\0';
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

/*
文字列は？：Medsys
sysdeM
*/