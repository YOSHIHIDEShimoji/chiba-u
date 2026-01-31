#include <stdio.h>

int count_number(char s[], int i);

int main(void)
{
	char s[100];
	int number;
	printf("数字文字列を入力してください： ");	scanf("%s", s);

	for(int i = 0; i <= 9; i++) {
		number = count_number(s, i);
		printf("%dの個数は%d個です.\n", i, number);
	}
	return 0;
}

int count_number(char s[], int i)
{
	int count = 0;
	for (int j = 0; j < 100; j++) {
		if (s[j] == '0' + i) {
			count++;
		}
	}
	return count;
}