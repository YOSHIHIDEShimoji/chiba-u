#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 256
#define MAX_PEAK_NUM 0.6

float *movingAverage(float *signal, int length, int K)
{
    /* NULLの代わりにmallocで動的配列を宣言すること*/
    float* averagedSignal = (float *)malloc(sizeof(float) * length);

    for (int i=0; i<length; i++)
    {
        float sum = 0.0;
        float count = 0.0;

        for (int j=i-K; j<=i+K; j++)
        {
            /* 条件式を設定すること */
            if (0 <= j && j <= length - 1)
            {
                sum += signal[j];
                count++;
            }
        }
        averagedSignal[i] = sum / count;
    }

    return averagedSignal;
}


int main(int argc, char **argv)
{
    /* 課題1: 心電図データの読み込み */
    int    dataLength = 0;
    
    FILE *fp = fopen("ecg4000.txt", "r");
    if (fp == NULL) {
        printf("Can't open data file.\n");
        return 1;
    }
    char buf[BUF_SIZE];
	fgets(buf,BUF_SIZE-1,fp);
	sscanf(buf,"%d",&dataLength);

    float* otime = (float *)malloc(sizeof(float) * dataLength);
    float* signal =  (float *)malloc(sizeof(float) * dataLength); 
    
    for (int i = 0; i < dataLength; i++) {
        fgets(buf, BUF_SIZE, fp);
        sscanf(buf, "%f,%f", &otime[i], &signal[i]);
    }
    
    /* 課題2: 移動平均処理の適用 */
    int K = atoi(argv[1]);
    float *averagedSignal = movingAverage(signal, dataLength, K);
    
    /* 課題3: ピーク処理の検出 */
    float* ds =  (float *)malloc(sizeof(float) * dataLength); 
    float* rpeak =  (float *)malloc(sizeof(float) * dataLength); 
    
    ds[0] = 0;
    for (int i = 1; i < dataLength; i++) {
        ds[i] = (averagedSignal[i] - averagedSignal[i - 1]) / (otime[i] - otime[i - 1]);
    }

    int j = 0;
    for (int i = 0; i < dataLength; i++) {
        if (averagedSignal[i] > MAX_PEAK_NUM) {
            if (ds[i] >= 0 && ds[i + 1] <= 0) {
                rpeak[j] = otime[i];
                printf("%d回目のR波のピーク時刻 %f[秒]\n",j + 1, rpeak[j]);
                j++;
            }
        }
    }

    float sum = 0;
    for (int i = 0; i < j - 1; i++) {
        sum += rpeak[i + 1] - rpeak[i];
    }
    float ave = sum / (j - 1);
    float hr = 60 / ave;
    
    printf("RR間隔の平均値 = %f[秒/回]\n", ave);
    printf("平均心拍数 = %f[回/分]\n", hr);

    free(otime);
    free(signal);
    free(averagedSignal);
    free(ds);
    free(rpeak);
    fclose(fp);
    return 0;
}
