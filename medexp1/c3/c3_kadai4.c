/*
 * 医工学実験1 C言語プログラミング2
 * 課題番号：4
 * 作成者：下地慶英
 * 作成日：2025/12/23
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void discrete_fourier_transform(int N, float signal[], float G_r[], float G_i[], float G_abs[])
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

void inverse_discrete_fourier_transform(int N, float G_r[], float G_i[], float g_r[], float g_i[])
{
    for (int n = 0; n < N; n++) {
        float g_r_tmp = 0;
        float g_i_tmp = 0;
        for (int i = 0; i <= N - 1; i++) {
            int k = i - N / 2;
            float theta = 2.0 * M_PI * n * k / N;
            g_r_tmp += G_r[i] * cos(theta) - G_i[i] * sin(theta);
            g_i_tmp += G_r[i] * sin(theta) + G_i[i] * cos(theta);
        }
        g_r[n] = g_r_tmp / N;
        g_i[n] = g_i_tmp / N;
        // printf("g_r[%d] = %f\tg_i[%d] = %f\t\n", n, g_r[n], n, g_i[n]);
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

    /* 離散フーリエ変換関数を呼び出し */
    float* G_r = (float *)malloc(sizeof(float) * dataLength);
    float* G_i = (float *)malloc(sizeof(float) * dataLength);
    float* G_abs = (float *)malloc(sizeof(float) * dataLength);

    discrete_fourier_transform(dataLength, signal, G_r, G_i, G_abs);

    /* 離散逆フーリエ変換関数を呼び出し */
    float* g_r = (float *)malloc(sizeof(float) * dataLength);
    float* g_i = (float *)malloc(sizeof(float) * dataLength);  

    inverse_discrete_fourier_transform(dataLength, G_r, G_i, g_r, g_i);

    /* printf */
    /*
    for (int i = 0; i < dataLength; i++) {
        printf("otime[%d] = %f\tg_r[%d] = %f\tg_i[%d] = %f\t\n", i, otime[i], i, g_r[i], i, g_i[i]);
    }
    */

    /* 誤差を計算 */
    float sse = 0;
    for (int i = 0; i < dataLength; i++) {
        sse += (signal[i] - g_r[i]) * (signal[i] - g_r[i]);
    }
    printf("元信号 %s と復元信号の二乗誤差和 SSE = %.10f\n",filename, sse);

    /* free and end */
    free(otime);
    free(signal);
    free(G_r);
    free(G_i);
    free(G_abs);
    free(g_r);
    free(g_i);
    return 0;
}
