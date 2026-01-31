#include <stdio.h>

int main(void)
{
	char s[70], *ptr1 = s;
	int i = 0;

	printf("Enter the strings: ");
	scanf("%s", s);
	
	while (s[i]) {
		i++;
		ptr1++;
	}

	while (--i >= 0) {
		putchar(*(--ptr1));
	}
	putchar('\n');

	return 0;
}

/*
Enter the strings: Shimoji
ijomihS
*/