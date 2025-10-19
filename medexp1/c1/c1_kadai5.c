#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 256
#define M 5

void cal_resp_ma(int i, int resp_ary[], float resp_ma[], int etime_ary[]);

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
    int *etime_ary, *resp_ary;
    etime_ary = (int *)malloc(sizeof(int) * N);
    resp_ary = (int *)malloc(sizeof(int) * N);

    /* read data */
    int etime = 0, resp = 0;
    for (int i = 0; i < N; i ++) {
        fgets(buf, BUF_SIZE - 1, fp);
        sscanf(buf, "%d,%d\n", &etime, &resp);
        etime_ary[i] = etime;
        resp_ary[i] = resp;
    }

    /* cal mean */
    double mean = 0, sum = 0;
    for (int i = 0; i < N; i++) {
        sum += resp_ary[i];
    }
    mean = sum / N;

    /* define array */
    float *resp_ma;
    resp_ma = (float *)malloc(sizeof(float) * N);

    /* cal resp_ma */
    for (int i = M - 1; i <= N - 1; i++) {
        cal_resp_ma(i, resp_ary, resp_ma, etime_ary);
    }

    /* cal timing */
    double timing;
    for (int i = M - 1; i < N - 1; i++) {
        if (resp_ma[i] > mean && mean >= resp_ma[i + 1]) {
            // printf("debug: i = %d\n", i);
            timing = etime_ary[i] + (mean - resp_ma[i]) / (resp_ma[i + 1] - resp_ma[i]) * (etime_ary[i + 1] - etime_ary[i]);
            printf("%fms\n", timing);
        }   
    }

    /* free */
    free(etime_ary);
    free(resp_ary);
    free(resp_ma);
    
    /* ending */
    fclose(fp);
    return 0;
}

void cal_resp_ma(int i, int resp_ary[], float resp_ma[], int etime_ary[])
{
    float sum = 0;
    for (int k = 0; k < M; k++) {
        sum += resp_ary[i - k];
    }
    resp_ma[i] = sum / M;
}
