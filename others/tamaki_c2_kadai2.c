#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 256
#define MAX_PEAK_NUM 6

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

    free(otime);
    free(signal);
    free(averagedSignal);
    fclose(fp);
    return 0;
}
