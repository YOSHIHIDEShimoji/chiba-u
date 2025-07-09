#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	FILE *infile = fopen("inputdata.txt", "r"), 
		 *outfile = fopen("outputdata.csb", "w");	
	char s[40], ts[15], as[15];
	double time[51], signal_A[51], signal_B[51];


	if (infile == NULL) {
		printf("can't open file inputdata.txt\n");
		exit (1);
	}

	// time[51]の設定
	fgets(s, 39, infile);
	int t;
	for (t = 0; t < 51; t++) {
		fgets(s, 39, infile);
		int i, j;
		for (i = 0, j = 0; s[i] != '\t'; i++, j++) {
			ts[i] = s[i];
		}
		ts[i] = '\0';
		time[t] = atof(ts);
		// printf("%f\n", time[t]);
	}

	
	// signal_A[51]の設定
	fseek(infile, 0, 0);
	fgets(s, 39, infile);
	int a;
	for (a = 0; a < 51; a++) {
		fgets(s, 39, infile);
		int i, j;
		for (i = 0, j = 0; s[i] != '\t'; i++, j++) {
			as[i] = s[i];
		}
		as[i] = '\0';
		signal_A[a] = atof(as);
		printf("%f\n", signal_A[a]);
	}

}

