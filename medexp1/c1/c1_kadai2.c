#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 256

int main()
{
    /* open file */
    FILE *fp = fopen("c1_data.txt", "r");
    if (fp == NULL) {
        printf("Can't open data file.\n");
        return 1;
    }

    /* define N */
    int N;
    char buf[BUF_SIZE];
    fgets(buf, BUF_SIZE - 1, fp);
    sscanf(buf, "%d\n", &N);

    /* define array */
    int *etime = (int *)malloc(sizeof(int) * N);
    int *resp = (int *)malloc(sizeof(int) * N);
    
    /* read data */
    for (int i = 0; i < N; i ++) {
        fgets(buf, BUF_SIZE - 1, fp);
        sscanf(buf, "%d,%d\n", &etime[i], &resp[i]);
    }

    /* print */
    for (int i = 0; i < 5; i++) {
        printf("%dms:%d\n", etime[N - 1 - i], resp[N - 1 - i]);
    }
    
    /* free and ending */
    free(etime);
    free(resp);
    fclose(fp);
    return 0;
}
