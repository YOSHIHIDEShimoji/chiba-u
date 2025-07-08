#include <stdio.h>
#include <string.h>

struct sample {
	int i;
	char c;
	float f;
	char s[30];
};

int main(void)
{
	FILE *file;
	struct sample record1, record2;
	record1.c = 'a';
	record1.i = -1024;
	record1.f = 3.2e-4;
	strcpy(record1.s, "Hello");

	file = fopen("temp6.dat", "wb");
	fwrite((char *)&record1, sizeof(struct sample), 1, file);
	fclose(file);
	
	file = fopen("temp6.dat", "rb");
	fread((char *)&record2, sizeof(struct sample), 1, file);

	printf("record2.c = %c\n", record2.c);
	printf("record2.i = %d\n", record2.i);
	printf("record2.f = %f\n", record2.f);
	printf("record2.s = %s\n", record2.s);

	return 0;
}