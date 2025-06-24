#include <stdio.h>

void print_n(char str[], int number);

int main(void)
{
	char s[100];
	int i;
	printf("文字列は？：");
	scanf("%s", s);
	printf("繰り返し回数は？：");
	scanf("%d", &i);
	print_n(s, i);
	return 0;
}

void print_n(char str[], int number)
{
	for (int i = 0; i < number; i++) {
		printf("%s", str);
	}
	printf("\n");
}

/*
文字列は？：Medsys
繰り返し回数は？：4
MedsysMedsysMedsysMedsys
*/