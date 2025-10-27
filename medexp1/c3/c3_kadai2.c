/*
 * 医工学実験1 C言語プログラミング2
 * 課題番号：
 * 作成者：
 * 作成日：
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BUF_SIZE 256

void fourier_transform(int N, float signal[], float G_r[], float G_i[], float G_abs[])
{
    for (int i = 0; i < N; i++) {
        int k = i - N / 2;
        double G_r_sum = 0;
        double G_i_sum = 0;
        for (int n = 0; n <= N - 1; n++) {
            double theta = 2.0 * M_PI * n * k / N;
            G_r_sum += signal[n] * cos(theta);
            G_i_sum += -(signal[n] * sin(theta));
        }
        G_r[i] = G_r_sum;
        G_i[i] = G_i_sum;
        G_abs[i] = sqrt(G_r[i] * G_r[i] + G_i[i] * G_i[i]);
        // printf("G_r[%d] = %f\tG_i[%d] = %f\tG_abs[%d] = %f\n", k, G_r[i], k, G_i[i], k, G_abs[i]);
    }
}

int main(int argc, char **argv)
{
    /* 引数がなければエラー */
    char filename[256];
    if (argc >= 2) {
        strcpy(filename, argv[1]);
    } else {
        printf("信号入力信号波形のファイル名を引数として指定してください\n");
        return 1;
    }
    
    /* open file */
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Can't open data file.\n");
        return 1;
    }

    /* dataLength を定義 */
    int dataLength;
    char buf[BUF_SIZE];
    fgets(buf, BUF_SIZE - 1, fp);
    sscanf(buf, "%d\n", &dataLength);

    float* otime = (float *)malloc(sizeof(float) * dataLength);
    float* signal = (float *)malloc(sizeof(float) * dataLength);

    float otime_tmp;
    float signal_tmp;
    for (int i = 0; i < dataLength; i++) {
        fgets(buf, BUF_SIZE - 1, fp);
        sscanf(buf, "%f,%f\n", &otime_tmp, &signal_tmp);
        otime[i] = otime_tmp;
        signal[i] = signal_tmp;
        // printf("otime[%d] = %f\tsignal[%d] = %f\n", i, otime[i], i, signal[i]);
    }

    /* フーリエ変換関数を呼び出し */
    float* G_r = (float *)malloc(sizeof(float) * dataLength);
    float* G_i = (float *)malloc(sizeof(float) * dataLength);
    float* G_abs = (float *)malloc(sizeof(float) * dataLength);
    fourier_transform(dataLength, signal, G_r, G_i, G_abs);

    /* サンプリング間隔 dt, サンプリング周波数 f_s, 周波数スペクトルのメモリ間隔 df, ナイキスト周波数 f_max を定義 */
    float dt = (otime[dataLength - 1] - otime[0]) / (dataLength - 1);
    float f_s = 1 / dt;
    float df = 1 / (dataLength * dt);
    float f_max = f_s / 2;

    /* |G| が最大となる周波数 f_1 を求める */
    float max = G_abs[0];
    float f_1;
    for (int i = 1; i < dataLength; i++) {
        if (max <= G_abs[i]) {
            max = G_abs[i];
            f_1 = (i - dataLength / 2) * df;
        }
    }
    float bpm = f_1 * 60.0;
    printf("心拍数 = %f[回/分]\n", bpm);


    /* printf */
    // printf("サンプリング間隔 dt = %f[s]\n", dt);
    // printf("サンプリング周波数 f_s = %f[Hz]\n", f_s);
    // printf("周波数スペクトルのメモリ間隔 df = %f[Hz]\n", df);
    // printf("ナイキスト周波数 f_max = %f[Hz]\n", f_max);

    // printf("\nk\tf[Hz]\tG_r\tG_i\t|G|\n");
    // for (int i = 0; i < dataLength; i++) {
    //     int k = i - dataLength / 2;
    //     printf("%d\t%f\t%f\t%f\t%f\n", k, k * df, G_r[i], G_i[i], G_abs[i]);
    // }


    /* free and end */
    fclose(fp);
    free(otime);
    free(signal);
    free(G_r);
    free(G_i);
    free(G_abs);
    return 0;
}