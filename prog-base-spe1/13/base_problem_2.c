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
	struct sample record1[3] = {
		{'a', -1024, 3.2e-4, "Hello"},
		{'b', 1023, 5.9e3, "Good bye"},
		{'c', 528, 1.3e-4, "why?"}
	};

	struct sample record2[3];

	file = fopen("temp6.dat", "wb");
	fwrite((char *)&record1, sizeof(struct sample), 3, file);
	fclose(file);
	
	file = fopen("temp6.dat", "rb");
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

/*
record2[0].c = a
record2[0].i = -1024
record2[0].f = 0.000320
record2[0].s = Hello

record2[1].c = b
record2[1].i = 1023
record2[1].f = 5900.000000
record2[1].s = Good bye

record2[2].c = c
record2[2].i = 528
record2[2].f = 0.000130
record2[2].s = why?

*/