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