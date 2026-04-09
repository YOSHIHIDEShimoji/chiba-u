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

    /* sampling rate */
    float rate = (N - 1) * 1000.0 / (etime[N - 1] - etime[0]);

    /* max and min */
    float min = resp[0];
    float max = resp[0];
    for (int i = 1; i < N; i++) {
        if (resp[i] < min) min = resp[i];
        if (resp[i] > max) max = resp[i];
    }     
    
    /* mean */
    float sum = 0;
    for (int i = 0; i < N; i++) {
        sum += resp[i];
    }
    float mean = sum / N;

    /* print and free */
    printf("sampling rate = %.3f\n", rate);
    printf("max = %.0f\n", max);
    printf("min = %.0f\n", min);
    printf("mean = %.3f\n", mean);

    /* free and ending */
    free(etime);
    free(resp);
    fclose(fp);
    return 0;
}
