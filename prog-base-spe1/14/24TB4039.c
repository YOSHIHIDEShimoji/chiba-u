/*
Advanced Computer Programming I
24TB4039
Yoshihide Shimoji
2025/07/15
*/

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("24TB4039\nYoshihide Shimoji\n");

	FILE *infile = fopen("inputdata.txt", "r"), 
		 *outfile = fopen("outputdata.csv", "w");	
	char s[40], ts[15], as[15], bs[15];
	double time[51], a, b, c, d;


	if (infile == NULL) {
		printf("can't open file inputdata.txt\n");
		exit (1);
	}

	fgets(s, 39, infile);
	int t;
	for (t = 0; t < 51; t++) {
		fgets(s, 39, infile);

		int i = 0, j = 0;

		while (s[i] != '\t') {
			ts[j] = s[i];
			i++;	j++; 
		}
		ts[j] = '\0';
		time[t] = atof(ts);
		
		i++;	j = 0;
		while (s[i] != '\t') {
			as[j] = s[i];
			i++;	j++;
		}
		as[j] = '\0';
		a = atof(as);

		i++;	j = 0;
		while (s[i] != '\0') {
			bs[j] = s[i];
			i++;	j++;
		}
		bs[j] = '\0';
		b = atof(bs);

		c = a + b;
		d = a * a + b * b;

		fprintf(outfile, "%f,%f,%f\n", time[t], c, d);
	}
	
	fclose(infile);
	fclose(outfile);
	return 0;
}

