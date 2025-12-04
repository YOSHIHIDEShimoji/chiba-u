#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 256

#ifndef M           // gcc -DM=7 *.c で M = 7 に設定
#define M 5         // M のデフォルト値 5
#endif

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
    float *resp_ma = (float *)malloc(sizeof(float) * N);

    /* read data */
    for (int i = 0; i < N; i ++) {
        fgets(buf, BUF_SIZE - 1, fp);
        sscanf(buf, "%d,%d\n", &etime[i], &resp[i]);
    }

    /* cal resp_ma */
    for (int i = M - 1; i <= N - 1; i++) {
        float tmp = 0;
        for (int j = 0; j <= M - 1; j++) {
            tmp +=resp[i - j];
        }
        resp_ma[i] = tmp / M;
    }
    
    /* print the resp_ma[i] value for etime[i] between 1000[ms] and 1100[ms] */
    printf("etime\tresp_ma\n");
    for (int i = 0; i < N; i++) {
        if (1000 <= etime[i] && etime[i] <= 1100) {
            printf("%d\t%.3f\n", etime[i], resp_ma[i]);
        }
    }

    /* free and ending */
    free(etime);
    free(resp);
    free(resp_ma);
    fclose(fp);
    return 0;
}
