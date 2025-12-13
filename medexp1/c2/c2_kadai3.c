/*
 * 医工学実験1 C言語プログラミング2
 * 課題番号：3
 * 作成者：下地慶英
 * 作成日：2025/12/09
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_PEAK_NUM 0.6

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
    float* ds = (float *)malloc(sizeof(float) * dataLength);
    float* r_peak = (float *)malloc(sizeof(float) * dataLength);

    /* 導関数 ds を定義 */
    ds[0] = 0.0;
    for (int i = 1; i < dataLength; i++) {
        ds[i] = (averagedSignal[i] - averagedSignal[i - 1]) / (otime[i] - otime[i - 1]);
    }

    /* averagedSignal の最大値 max を求める */
    float max = averagedSignal[0];
    for (int i = 1; i < dataLength; i++) {
        if (max <= averagedSignal[i]) {
            max = averagedSignal[i];
        }
    }

    /* RR波のピーク時刻の配列 r_peak を定義して print */
    /* MAX_PEAK_NUM は使わず、max の 0.7 倍とする */
    printf("R波のピーク時刻\n");
    int count = 0;
    for (int i = 0; i < dataLength - 1; i++) {
        if (averagedSignal[i] > max * 0.7 && ds[i] >= 0 && ds[i + 1] <= 0) {
            r_peak[count] = otime[i];

            /* RR波のピーク時刻[s] */
            printf("peak %d\t%.3f[秒]\n",count + 1, r_peak[count]);
            count++;
        }
    }

    /* RR間隔の平均値[s/回]と平均心拍数[回/分] */
    float tmp = 0;
    for (int i = 0; i < count - 1; i++) {
        tmp += r_peak[i + 1] - r_peak[i];
    }
    float mean = tmp / (count - 1);
    float bpm = 60.0 / mean;

    printf("\nRR間隔の平均値 = %.3f[秒/回]\n", mean);
    printf("\n平均心拍数 = %.3f[回/分]\n", bpm);
    
    /* free and end */
    fclose(fp);
    free(otime);
    free(signal);
    free(averagedSignal);
    free(ds);
    free(r_peak);
    return 0;
}