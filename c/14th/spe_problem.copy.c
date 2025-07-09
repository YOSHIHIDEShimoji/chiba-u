#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	FILE *infile = fopen("inputdata.txt", "r"), 
		 *outfile = fopen("outputdata.csb", "w");	
	char s[40], ts[15], as[15];
	double time[51];


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
			ts[j] = s[i];
		}
		ts[j] = '\0';
		time[t] = atof(ts);
		// printf("%f\n", time[t][0]);
	
		i++; j++;
		for (; s[i] != '\t'; i++, j++) {
			as[j] = s[i];
		}
		as[j] = '\0';
		time[t] = atof(as);
		printf("%f\n", time[t]);
	}



}

