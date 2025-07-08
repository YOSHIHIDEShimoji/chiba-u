#include <stdio.h>
#include <string.h>

struct sample {
	char c;
	int i;
	float f;
	char s[30];
};

int main(void)
{
	FILE *file;
struct sample record1[3], record2[3];

	record1[0].c = 'a';
	record1[0].i = -1024;
	record1[0].f = 3.2e-4f;
	strcpy(record1[0].s, "Hello");

	record1[1].c = 'b';
	record1[1].i = 1023;
	record1[1].f = 5.9e3f;
	strcpy(record1[1].s, "Good bye");

	record1[2].c = 'c';
	record1[2].i = 528;
	record1[2].f = 1.3e-4f;
	strcpy(record1[2].s, "why?");

	file = fopen("temp6.dat", "w");
	fwrite((char *)&record1, sizeof(struct sample), 3, file);
	fclose(file);
	
	file = fopen("temp6.dat", "r");
	fread((char *)&record2, sizeof(struct sample), 3, file);

	for(int i = 0; i < 3; i++) {
		printf("record2[%d].c = %c\n", i, record2[i].c);
		printf("record2[%d].i = %d\n", i, record2[i].i);
		printf("record2[%d].f = %f\n", i, record2[i].f);
		printf("record2[%d].s = %s\n", i, record2[i].s);
		printf("\n");
	}
	return 0;
}