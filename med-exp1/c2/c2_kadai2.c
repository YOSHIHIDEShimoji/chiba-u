/*
 * 医工学実験1 C言語プログラミング2
 * 課題番号：2
 * 作成者：下地慶英
 * 作成日：2025/12/09
 */

#include <stdio.h>
#include <stdlib.h>

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
        printf("averagedSignal[%d] = %f\n", i, averagedSignal[i]);
    }

    return averagedSignal;
}

int main(int argc, char **argv)
{
    /* 引数がなければエラー */
    int K;
    if (argc >= 2) {
        K = atoi(argv[1]);
    } else {
        printf("Usage: %s <moving_average_size>\n", argv[0]);
        return 1;
    }
    
    /* 課題1: 心電図データの読み込み */
    /* open file */
    FILE *fp = fopen("ecg4000.txt", "r");
    if (fp == NULL) {
        printf("Can't open data file.\n");
        return 1;
    }
    
    /* define dataLength */
    int dataLength = 0;
    char buf[256];
    fgets(buf, 256 - 1, fp);
    sscanf(buf, "%d\n", &dataLength);

    /* define array */
    float* otime = (float *)malloc(sizeof(float) * dataLength);
    float* signal = (float *)malloc(sizeof(float) * dataLength);

    for (int i = 0; i < dataLength; i++) {
        fgets(buf, 256 - 1, fp);
        sscanf(buf, "%f,%f\n", &otime[i], &signal[i]);
    }

    /* 課題2: 移動平均処理の適用 */
    float *averagedSignal = movingAverage(signal, dataLength, K);

    /* 課題3: ピーク処理の検出 */

    /* free and end */
    fclose(fp);
    free(otime);
    free(signal);
    free(averagedSignal);
    return 0;
}