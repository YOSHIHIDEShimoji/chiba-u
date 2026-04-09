/*
 * 医工学実験1 C言語プログラミング2
 * 課題番号：4
 * 作成者：下地慶英
 * 作成日：2025/12/09
 */

#include <stdio.h>
#include <stdlib.h>

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
    /* ecg4000.txt の読み込み */
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

    /* ecg400.txt の読み込み */
    FILE *fp_short = fopen("ecg400.txt", "r");
    if (fp_short == NULL) {
        printf("Can't open data file.\n");
        return 1;
    }

    /* define dataLength_short */
    int dataLength_short = 0;
    fgets(buf, 256 - 1, fp_short);
    sscanf(buf, "%d\n", &dataLength_short);

    /* define array */
    float * otime_short = (float *)malloc(sizeof(float) * dataLength_short);
    float * signal_short = (float *)malloc(sizeof(float) * dataLength_short);

    for (int i = 0; i < dataLength_short; i++) {
        fgets(buf, 256 - 1, fp_short);
        sscanf(buf, "%f,%f\n", &otime_short[i], &signal_short[i]);
    }

    /* 課題2: 移動平均処理の適用 */
    float *averagedSignal = movingAverage(signal, dataLength, K);
    float *averagedSignal_short = movingAverage(signal_short, dataLength, K);
    
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

    /* RR波のピーク時刻の配列 r_peak を定義 */
    /* MAX_PEAK_NUM は使わず、max の 0.7 倍とする */
    int count = 0;
    for (int i = 0; i < dataLength - 1; i++) {
        if (averagedSignal[i] > max * 0.7 && ds[i] >= 0 && ds[i + 1] <= 0) {
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

    // printf("\nRR間隔の平均値 = %f[秒/回]\n", mean);
    // printf("\n平均心拍数 = %f[回/分]\n", bpm);

    /* 課題4 */
    /* r_xyの定義 */
    float* r_xy = (float *)malloc(sizeof(float) * (dataLength - dataLength_short + 1));
    for (int i = 0; i <= dataLength - dataLength_short; i++) {
        float sigma = 0.0;
        for (int j = 0; j < dataLength_short; j++) {
            sigma += averagedSignal_short[j] * averagedSignal[j + i];
        }
        r_xy[i] = sigma;
    }

    /* r_xyの極大の検出 */
    float* dr = (float *)malloc(sizeof(float) * (dataLength - dataLength_short + 1));
    float* r_xy_peak = (float *)malloc(sizeof(float) * (dataLength - dataLength_short + 1));

    /* 導関数 dr を定義*/
    dr[0] = 0.0;
    for (int i = 1; i < dataLength; i++) {
        dr[i] = (r_xy[i] - r_xy[i - 1]) / (otime[i] - otime[i - 1]);
    }

    /* r_xy の最大値 max を求める */
    float max_short = r_xy[0];
    for (int i = 1; i < dataLength - dataLength_short; i++) {
        if (max_short <= r_xy[i]) {
            max_short = r_xy[i];
        }
    }
    
    /* r_xy のピーク時刻の配列 r_xy_peak を定義 */
    printf("心電図信号に関係する i\n");
    int count_short = 0;
    for (int i = 0; i < dataLength - dataLength_short; i++) {
        if (r_xy[i] > max_short * 0.7 && dr[i] >= 0 && dr[i + 1] <= 0) {
            r_xy_peak[count_short] = otime[i];

            /* 心電図と関係する i */
            printf("i = %d\n", i);
            count_short++;
        }
    }

    /* RR間隔の平均値[s/回]と平均心拍数[回/分] */
    printf("\n%d個のRR間隔\n", count_short - 1);
    float tmp_short = 0;
    for (int i = 0; i < count_short - 1; i++) {
        float interval = r_xy_peak[i + 1] - r_xy_peak[i];
        printf("%.3f[秒]\n", interval);
        tmp_short += interval;
    }
    float mean_short = tmp_short / (count_short - 1);
    float bpm_short = 60.0 / mean_short;
    
    printf("\n%d個のRR間隔の平均値 = %.3f[秒/回]\n", count_short - 1, mean_short);
    printf("\n平均心拍数 = %.3f[回/分]\n", bpm_short);

    /* free and end */
    fclose(fp);
    fclose(fp_short);
    free(otime);
    free(signal);
    free(otime_short);
    free(signal_short);
    free(averagedSignal);
    free(averagedSignal_short);
    free(ds);
    free(r_peak);
    free(dr);
    free(r_xy_peak);
    free(r_xy);
    return 0;
}