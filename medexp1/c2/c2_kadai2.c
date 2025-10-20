/*
 * 医工学実験1 C言語プログラミング2
 * 課題番号：
 * 作成者：
 * 作成日：
 */

#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 256
#define MAX_PEAK_NUM 6

float *movingAverage(float *signal, int length, int K)
{
    /* NULLの代わりにmallocで動的配列を宣言すること*/
    float* averagedSignal = (float *)malloc(sizeof(float) * length);

    for (int i = 0; i < length; i++) {
        float sum = 0.0;
        float count = 0.0;

        for (int j = i - K; j <= i + K; j++) {
            /* jが配列の範囲内かどうかの判定 */
            if (0 <= j && j <= length - 1) {
                sum += signal[j];
                count++;
            }
            
        }
        averagedSignal[i] = sum / count;
        printf("%f\n", averagedSignal[i]);
    }

    return averagedSignal;
}


int main(int argc, char **argv)
{
    /* 課題1: 心電図データの読み込み */
    int    dataLength = 0;
    float* otime = NULL;
    float* signal = NULL;

    /* open file */
    FILE *fp = fopen("ecg4000.txt", "r");
    if (fp == NULL) {
        printf("Can't open data file.\n");
        return 1;
    }

    /* define dataLength */
    char buf[BUF_SIZE];
    fgets(buf, BUF_SIZE - 1, fp);
    sscanf(buf, "%d\n", &dataLength);
    // printf("%d\n",dataLength);

    /* define array */
    otime = (float *)malloc(sizeof(float) * dataLength);
    signal = (float *)malloc(sizeof(float) * dataLength);

    float otime_value;
    float signal_value;
    for (int i = 0; i < dataLength; i++) {
        fgets(buf, BUF_SIZE - 1, fp);
        sscanf(buf, "%f,%f\n", &otime_value, &signal_value);
        otime[i] = otime_value;
        signal[i] = signal_value;
        // printf("otime[%d] = %f\tsignal[%d] = %f\n", i, otime[i], i, signal[i]);
    }

    /* 課題2: 移動平均処理の適用 */
    int K = 1;
    
    if (argc >= 2) {
        K = atoi(argv[1]);
    } else {
        printf("移動平均のサイズ K を入力してください\n");
    }
    
    float *averagedSignal = movingAverage(signal, dataLength, K);
    // printf("%d\n", K);
    


    /* 課題3: ピーク処理の検出 */

    /* free and end */
    fclose(fp);
    free(otime);
    free(signal);
    free(averagedSignal);
    return 0;
}