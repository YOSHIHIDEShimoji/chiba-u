/*
 * 医工学実験1 C言語プログラミング2
 * 課題番号：
 * 作成者：
 * 作成日：
 */

#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 256
#define MAX_PEAK_NUM 0.6
#define MAX_PEAK_NUM_2 6

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
        // printf("%f\n", averagedSignal[i]);
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

    /* ecg400 の読み込み */
    int dataLength_short;
    FILE *fp_short = fopen("ecg400.txt", "r");
    if (fp_short == NULL) {
        printf("Can't open data file.\n");
        return 1;
    }
    fgets(buf, BUF_SIZE - 1, fp_short);
    sscanf(buf, "%d\n", &dataLength_short);
    float * otime_short = (float *)malloc(sizeof(float) * dataLength_short);
    float * signal_short = (float *)malloc(sizeof(float) * dataLength_short);
    for (int i = 0; i < dataLength_short; i++) {
        fgets(buf, BUF_SIZE - 1, fp_short);
        sscanf(buf, "%f,%f\n", &otime_value, &signal_value);
        otime_short[i] = otime_value;
        signal_short[i] = signal_value;
        // printf("otime_short[%d] = %f\tsignal_short[%d] = %f\n", i, otime_short[i], i, signal_short[i]);
    }

    /* 課題2: 移動平均処理の適用 */
    int K = 1;
    
    if (argc >= 2) {
        K = atoi(argv[1]);
    } else {
        printf("移動平均のサイズ K を引数として指定してください\n");
        return 1;
    }
    
    float *averagedSignal = movingAverage(signal, dataLength, K);
    // printf("%d\n", K);
    
        /* 課題3: ピーク処理の検出 */
    float* ds = (float *)malloc(sizeof(float) * dataLength);
    float* r_peak = (float *)malloc(sizeof(float) * dataLength);

    /* 導関数 ds を定義 */
    ds[0] = 0.0;
    for (int j = 1; j < dataLength; j++) {
        ds[j] = (averagedSignal[j] - averagedSignal[j - 1]) * j / otime[j];
    }

    /* RR波のピーク時刻の配列 r_peak を定義 */
    int count = 0;
    for (int i = 0; i < dataLength; i++) {
        if (averagedSignal[i] > MAX_PEAK_NUM && ds[i] >= 0 && ds[i + 1] <= 0) {
            // printf("%d\t%f\t%f\n", i, otime[i], averagedSignal[i]);
            r_peak[count] = otime[i];
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
    // printf("\nRR間隔の平均値 = %f[秒/回]\n\n平均心拍数 = %f[回/分]\n", mean, bpm);
    
    /* 課題4 */
    /* r_xyの定義 */
    float * r_xy = (float *)malloc(sizeof(float) * (dataLength - dataLength_short + 1));
    for (int i = 0; i <= dataLength - dataLength_short; i++) {
        double sigma = 0.0;
        for (int j = 0; j < dataLength_short; j++) {
            sigma += signal_short[j] * averagedSignal[j + i];
        }
        r_xy[i] = sigma;
        // printf("%f\n", r_xy[i]);
    }

    /* r_xyの極大の検出 */
    float* dr = (float *)malloc(sizeof(float) * dataLength_short);
    float* r_xy_peak = (float *)malloc(sizeof(float) * dataLength_short);

    /* 導関数 dr を定義*/
    for (int j = 0; j < dataLength; j++) {
        dr[j] = (r_xy[j] - r_xy[j - 1]) * j / otime[j];
    }

    /* r_xy のピーク時刻の配列 r_xy_peak を定義 */
    printf("心電図に関係する i\n");
    int count_2 = 0;
    for (int i = 0; i < dataLength - dataLength_short; i++) {
        if (r_xy[i] > MAX_PEAK_NUM_2 && dr[i] >= 0 && dr[i + 1] <= 0) {
            // printf("%d\t%f\t%f\n", i, otime[i], r_xy[i]);
            r_xy_peak[count_2] = otime[i];

            /* 心電図と関係する i */
            printf("i = %d\n", i);
            count_2++;
        }
    }

    /* RR間隔の平均値[s/回]と平均心拍数[回/分] */
    printf("\n%d個のRR間隔\n", count_2 - 1);
    float tmp_2 = 0;
    for (int i = 0; i < count_2 - 1; i++) {
        float interval = r_xy_peak[i + 1] - r_xy_peak[i];
        printf("%f[秒]\n", interval);
        tmp_2 += interval;
    }
    float mean_2 = tmp_2 / (count_2 - 1);
    float bpm_2 = 60.0 / mean_2;
    printf("\n%d個のRR間隔の平均値 = %f[秒/回]\n\n平均心拍数 = %f[回/分]\n", count_2 - 1, mean_2, bpm_2);

    /* free and end */
    fclose(fp);
    free(otime);
    free(signal);
    free(averagedSignal);
    free(ds);
    free(r_peak);
    free(dr);
    free(r_xy_peak);
    free(otime_short);
    free(signal_short);
    free(r_xy);
    return 0;
}