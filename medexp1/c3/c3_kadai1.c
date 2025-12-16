/*
 * 医工学実験1 C言語プログラミング2
 * 課題番号：1
 * 作成者：下地慶英
 * 作成日：2025/12/23
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void fourier_transform(int N, float signal[], float G_r[], float G_i[], float G_abs[])
{
    for (int i = 0; i < N; i++) {
        int k = i - N / 2;
        float G_r_tmp = 0;
        float G_i_tmp = 0;
        for (int n = 0; n <= N - 1; n++) {
            float theta = 2.0 * M_PI * n * k / N;
            G_r_tmp += signal[n] * cos(theta);
            G_i_tmp += -(signal[n] * sin(theta));
        }
        G_r[i] = G_r_tmp;
        G_i[i] = G_i_tmp;
        G_abs[i] = sqrt(G_r[i] * G_r[i] + G_i[i] * G_i[i]);
    }
}

int main(int argc, char **argv)
{
    /* 引数がなければエラー */
    char filename[256];
    if (argc >= 2) {
        strcpy(filename, argv[1]);
    } else {
        printf("Usage: %s <singal_data_file>\n", argv[0]);
        return 1;
    }
    
    /* open file */
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Can't open data file.\n");
        return 1;
    }

    /* dataLength, otime[], sinal[] を定義 */
    int dataLength;
    char buf[256];
    fgets(buf, 256 - 1, fp);
    sscanf(buf, "%d\n", &dataLength);

    float* otime = (float *)malloc(sizeof(float) * dataLength);
    float* signal = (float *)malloc(sizeof(float) * dataLength);

    for (int i = 0; i < dataLength; i++) {
        fgets(buf, 256 - 1, fp);
        sscanf(buf, "%f,%f\n", &otime[i], &signal[i]);
    }

    fclose(fp);

    /* フーリエ変換関数を呼び出し */
    float* G_r = (float *)malloc(sizeof(float) * dataLength);
    float* G_i = (float *)malloc(sizeof(float) * dataLength);
    float* G_abs = (float *)malloc(sizeof(float) * dataLength);

    fourier_transform(dataLength, signal, G_r, G_i, G_abs);

    /* dt: サンプリング間隔
     * f_s: サンプリング周波数
     * df: 周波数スペクトルのメモリ間隔
     * f_max: ナイキスト周波数
     */
    float dt = (otime[dataLength - 1] - otime[0]) / (dataLength - 1);
    float f_s = 1.0 / dt;
    float df = 1.0 / (dataLength * dt);
    float f_max = f_s / 2.0;

    /* printf */
    printf("サンプリング間隔 dt = %.5f[s]\n", dt);
    printf("サンプリング周波数 f_s = %.3f[Hz]\n", f_s);
    printf("周波数スペクトルのメモリ間隔 df = %.3f[Hz]\n", df);
    printf("ナイキスト周波数 f_max = %.3f[Hz]\n", f_max);

    printf("\nk\tf[k]\tG_r[k]\tG_i[k]\t|G[k]|\n");
    for (int i = 0; i < dataLength; i++) {
        int k = i - dataLength / 2;
        printf("%d\t%f\t%f\t%f\t%f\n", k, k * df, G_r[i], G_i[i], G_abs[i]);
    }


    /* free and end */
    free(otime);
    free(signal);
    free(G_r);
    free(G_i);
    free(G_abs);
    return 0;
}