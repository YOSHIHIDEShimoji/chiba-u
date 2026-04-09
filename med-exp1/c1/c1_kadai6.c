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

    /* cal mean */
    float mean = 0, sum = 0;
    for (int i = 0; i < N; i++) {
        sum += resp[i];
    }
    mean = sum / N;

    /* cal resp_ma */
    for (int i = M - 1; i <= N - 1; i++) {
        float tmp = 0;
        for (int j = 0; j <= M - 1; j++) {
            tmp +=resp[i - j];
        }
        resp_ma[i] = tmp / M;
    }

    /* cal timing */
    float t[N];
    int count = 0;
    for (int i = M - 1; i < N - 1; i++) {
        if (resp_ma[i] > mean && mean >= resp_ma[i + 1]) {
            float timing = etime[i] + (mean - resp_ma[i]) / (resp_ma[i + 1] - resp_ma[i]) * (etime[i + 1] - etime[i]);
            t[count] = timing;
            count++;
        }   
    }

    /* cal T */
    float _tmp = 0;
    for (int i = 0; i < count - 1; i++) {
        printf("%d回目の呼吸周期： %.3fms\n", i + 1, t[i + 1] - t[i]);
        _tmp += t[i + 1] - t[i];
    }
    float T = _tmp / (count - 1);
    float bpm = 60.0 * 1000.0 / T;

    printf("\n呼吸周期の平均 = %.3fms\n", T);
    printf("呼吸数 = %.3fbpm\n", bpm);

    /* free and ending */
    free(etime);
    free(resp);
    free(resp_ma);
    fclose(fp);
    return 0;
}
