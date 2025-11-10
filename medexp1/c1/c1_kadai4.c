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

    /* array を定義 */
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

    /* find i between 1000 and 1100 */
    int count = 0;
    for (int i = 0; i < N; i++) {
        if (1000 <= etime_ary[i] && etime_ary[i] <= 1100) {
            count++;
        }
    }

    /* array を定義 */
    float *resp_ma;
    resp_ma = (float *)malloc(sizeof(float) * N);

    /* index を定義 */
    printf("etime\tresp_ma\n");

    for (int i = 0; i < N; i++) {
        if (i >= M - 1 && 1000 <= etime_ary[i] && etime_ary[i] <= 1100) {
            cal_resp_ma(i, resp_ary, resp_ma, etime_ary);
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
    long sum = 0;
    for (int k = 0; k < M; k++) {
        sum += resp_ary[i - k];
    }
    resp_ma[i] = (float)sum / (float)M;

    printf("%d\t%f\n", etime_ary[i], resp_ma[i]);
}
