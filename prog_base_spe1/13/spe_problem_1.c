#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	FILE *file;
	if ((file = fopen("test.txt", "r")) == NULL) {
		printf("can't open file test.txt\n");
		exit (1);
	}

	fseek(file, -1L, 2);
	do {
		putchar(fgetc(file));
	} while (!fseek(file, -2L, 1));
	printf("\n");
	
	return 0;
}

/*
09876
54321
*/