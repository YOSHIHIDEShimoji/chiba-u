#include <stdio.h>

int main(void)
{
	char *ptr1, *ptr2;
	ptr1 = ptr2 = "Chiba";

	while (*ptr2) {
		ptr2++;
	}

	while (--ptr2 >= ptr1) {
		putchar(*ptr2);
	}
	putchar('\n');
}