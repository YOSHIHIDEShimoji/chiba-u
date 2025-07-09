#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	FILE *infile = fopen("inputdata.txt", "r"), 
		 *outfile = fopen("outputdata.csb", "w");	
	char s[40], ts[15];

	if (infile == NULL) {
		printf("can't open file inputdata.txt\n");
		exit (1);
	}

	fgets(s, 39, infile);
	for (int i = 0; i < 51; i++) {
		fgets(s, 39, infile);
		printf("%s", s);
	}
	
	int i, j;
	for (i = 0, j = 0; s[i] != '\t'; j++, i++) {
		ts[i] = s[i];
	}
	ts[i] = '\0';
	printf("%s\n", ts);

}