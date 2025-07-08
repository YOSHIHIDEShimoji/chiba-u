#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	FILE *file;
	int count_char = 0, count_line = 0;
	char filename[256], string[1024];
	printf("input filename = ");
	scanf("%s", filename);

	if ((file = fopen(filename, "r")) == NULL) {
		printf("can't open file %s\n", filename);
		exit (1);
	}

	while (fgetc(file) != EOF) {
		count_char++;
	}
	fseek(file, 0L, 0);
	while (fgets(string, 1024, file) != NULL) {
		count_line++;
	}
	printf("chars = %d\nlines = %d\n", count_char, count_line);
	return 0;
}