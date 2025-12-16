/*
 * 医工学実験1 C言語プログラミング2
 * 課題番号：2
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

    /* 以下を定義
     * dt: サンプリング間隔
     * f_s: サンプリング周波数
     * df: 周波数スペクトルのメモリ間隔
     * f_max: ナイキスト周波数
     */
    float dt = (otime[dataLength - 1] - otime[0]) / (dataLength - 1);
    float f_s = 1.0 / dt;
    float df = 1.0 / (dataLength * dt);
    float f_max = f_s / 2.0;

    /* |G| が最大となる周波数 f_1[Hz] を求める */
    float max = G_abs[0];
    float f_1;
    for (int i = 1; i < dataLength; i++) {
        if (max <= G_abs[i]) {
            max = G_abs[i];
            f_1 = (i - dataLength / 2) * df;
        }
    }
    float bpm = f_1 * 60.0;
    printf("心拍数 = %.3f[回/分]\n", bpm);

    /* free and end */
    free(otime);
    free(signal);
    free(G_r);
    free(G_i);
    free(G_abs);
    return 0;
}