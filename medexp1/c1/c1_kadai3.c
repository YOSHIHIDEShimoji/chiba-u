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

    /* calculation, print, free */
    double rate, max = 0, min = 0, mean, sum;
    
    /* sampling rate */
    rate = (N - 1) * 1000.0 / (etime_ary[N - 1] - etime_ary[0]);

    /* max and min */
    min = max = resp_ary[0];
    for (int i = 1; i < N; i++) {
        if (resp_ary[i] < min) min = resp_ary[i];
        if (resp_ary[i] > max) max = resp_ary[i];
    }     
    
    /* mean */
    for (int i = 0; i < N; i++) {
        sum += resp_ary[i];
    }
    mean = sum / N;

    /* print and free */
    printf("sampling rate = %f\n", rate);
    printf("max = %f\n", max);
    printf("min = %f\n", min);
    printf("mean = %f\n", mean);

    free(etime_ary);
    free(resp_ary);
    
    /* ending */
    fclose(fp);
    return 0;
}
