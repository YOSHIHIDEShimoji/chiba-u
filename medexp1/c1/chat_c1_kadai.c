#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int etime;  // ms
    int resp;   // amplitude
} Sample;

int main(int argc, char **argv)
{
    const char *path = (argc >= 2) ? argv[1] : "c1_data.txt";
    FILE *fp = fopen(path, "r");
    if (!fp) {
        fprintf(stderr, "Error: cannot open '%s'\n", path);
        return 1;
    }

    /* 課題1: 先頭行で N を読み取り、以後は etime,resp を読む */
    int N = 0;
    if (fscanf(fp, "%d", &N) != 1 || N <= 0) {
        fprintf(stderr, "Error: failed to read N or N<=0\n");
        fclose(fp);
        return 1;
    }

    /* 課題2: 動的配列を確保して全サンプルを保存 */
    Sample *dat = (Sample *)malloc(sizeof(Sample) * N);
    if (!dat) {
        fprintf(stderr, "Error: malloc failed for %d samples\n", N);
        fclose(fp);
        return 1;
    }

    for (int i = 0; i < N; i++) {
        int et, rsp;
        if (fscanf(fp, "%d,%d", &et, &rsp) != 2) {
            fprintf(stderr, "Error: malformed data at line %d\n", i + 2);
            free(dat);
            fclose(fp);
            return 1;
        }
        dat[i].etime = et;
        dat[i].resp  = rsp;
    }
    fclose(fp);

    /* 課題1: 先頭3つ、末尾3つのみ表示（例の書式: 9ms:317 など） */
    printf("=== Kadai 1 ===\n");
    for (int i = 0; i < 3 && i < N; i++) {
        printf("%dms:%d\n", dat[i].etime, dat[i].resp);
    }
    for (int k = 3; k >= 1; k--) {
        int i = N - k;
        if (i >= 0) printf("%dms:%d\n", dat[i].etime, dat[i].resp);
    }

    /* 課題2: 末尾から5データを逆順表示 */
    printf("=== Kadai 2 ===\n");
    int last = (N >= 5) ? 5 : N;
    for (int j = 0; j < last; j++) {
        int i = N - 1 - j;
        printf("%dms:%d\n", dat[i].etime, dat[i].resp);
    }

    /* 課題3: サンプリングレート，最大，最小，平均 */
    int resp_min = dat[0].resp, resp_max = dat[0].resp;
    long long resp_sum = 0;
    for (int i = 0; i < N; i++) {
        if (dat[i].resp < resp_min) resp_min = dat[i].resp;
        if (dat[i].resp > resp_max) resp_max = dat[i].resp;
        resp_sum += dat[i].resp;
    }
    double resp_mean = (double)resp_sum / (double)N;

    /* 非等間隔サンプルなので (N-1) / Δt でレートを見積もる */
    double duration_ms = (double)(dat[N-1].etime - dat[0].etime); // ms
    double fs = (duration_ms > 0.0) ? ((double)(N - 1)) / (duration_ms / 1000.0) : 0.0;

    printf("=== Kadai 3 ===\n");
    printf("sampling_rate=%.6g Hz, resp_max=%d, resp_min=%d, resp_mean=%.6f\n",
           fs, resp_max, resp_min, resp_mean);

    /* 課題4: 移動平均 (M=5) を float 配列で計算し，1.0–1.1 s を表示 */
    const int M = 5;
    float *ma = (float *)calloc(N, sizeof(float));
    if (!ma) {
        fprintf(stderr, "Error: calloc failed for moving average\n");
        free(dat);
        return 1;
    }
    for (int i = M - 1; i < N; i++) {
        long sum5 = 0;
        for (int j = 0; j < M; j++) sum5 += dat[i - j].resp;
        ma[i] = (float)sum5 / (float)M;
    }

    printf("=== Kadai 4 (M=5 moving average, 1.0–1.1s) ===\n");
    for (int i = 0; i < N; i++) {
        if (dat[i].etime >= 1000 && dat[i].etime <= 1100) {
            /* 指導書のサンプルに近い書式: i, etime, resp_ma */
            printf("%d\t%d\t%.3f\n", i, dat[i].etime, (double)ma[i]);
        }
    }

    /* 課題5: 移動平均が「平均値（rawの全体平均）」を上から下に横切る時刻（ms） */
    /* 直線補間で交差時刻をサブサンプル精度で算出 */
    double *cross = (double *)malloc(sizeof(double) * N); // 上限Nで十分
    int C = 0;
    for (int i = M; i < N; i++) {
        double y1 = ma[i - 1];
        double y2 = ma[i];
        if (y1 >= resp_mean && y2 < resp_mean) {
            int t1 = dat[i - 1].etime;
            int t2 = dat[i].etime;
            double frac = (resp_mean - y1) / (y2 - y1); // y2<y1なので分母<0
            double tc = (double)t1 + frac * (double)(t2 - t1);
            cross[C++] = tc;
        }
    }

    printf("=== Kadai 5 (down-cross times of mean) ===\n");
    for (int i = 0; i < C; i++) {
        printf("%.3f ms\n", cross[i]);
    }

    /* 課題6: 交差時刻の周期 -> 平均周期 -> 呼吸数(bpm) */
    double Tlist[8];   /* 最大8周期ぶん保持（平均は指導書どおり最大8周期で計算） */
    int use = 0;

    /* 交差時刻 cross[0..C-1] から隣接差を周期として取得（ms） */
    for (int i = 1; i < C && use < 8; i++) {
        double T = cross[i] - cross[i - 1];  /* ms */
        if (T > 0.0) {
            Tlist[use++] = T;
        }
    }

    /* 1回目〜7回目を個別に表示（存在するところまで） */
    printf("=== Kadai 6 ===\n");
    int show = (use < 7) ? use : 7;
    for (int i = 0; i < show; i++) {
        printf("T%d = %.6f ms\n", i + 1, Tlist[i]);
    }

    /* 平均周期（最大8周期で平均）と呼吸数 */
    double Tsum = 0.0;
    for (int i = 0; i < use; i++) Tsum += Tlist[i];
    double Tmean_ms = (use > 0) ? (Tsum / (double)use) : 0.0;
    double bpm = (Tmean_ms > 0.0) ? (60000.0 / Tmean_ms) : 0.0;

    printf("cycles_used=%d, mean_period=%.6f ms, respiratory_rate=%.6f bpm\n", use, Tmean_ms, bpm);

    /* 後始末 */
    free(ma);
    free(cross);
    free(dat);
    return 0;
}
