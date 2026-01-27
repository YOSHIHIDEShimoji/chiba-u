/*
 * 医工学実験１ C言語プログラミング４
 * 巡回セールスマン問題を2-opt法で解く
 * 課題番号：24TB4039
 * 作成者：下地慶英
 * 作成日：2026/01/27
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>

#ifndef N           /* "gcc -DN=100 -lm *.c " で "#define N 100" とする */
#define N 100       /* デフォルト都市の数 */
#endif

/* 都市の座標構造体 */
struct City {
    int x, y;
};

/* 問題情報構造体 */
struct TSP {
    struct City city[N];    /* 都市の位置 */
    int order[N];           /* 巡回順 */
    float cost;             /* 総移動距離 */
};

/* グローバル変数：最高記録保持用 */
float cost_min = FLT_MAX;   /* 初期値は最大値にしておく */
int best_order[N];          /* 最短経路を保持するための配列 */
unsigned long long trial_count = 0; /* 試行回数カウンタ */

/* 関数の宣言 */
void ReadData(struct TSP *tsp);
void InitialOrder(struct TSP *tsp);
void SDM(struct TSP *tsp);
void Infinite_Search(struct TSP *tsp);
void CalcCost(struct TSP *tsp);
float CalcDistance(struct City a, struct City b);
float CalcCostOrder(struct TSP *tsp, int order[]);
void TwoOpt(const int currentOrder[N], int changedOrder[N], int x1, int x2);
int UpdateOrder(struct TSP *tsp, int x1, int x2);
void ShowResult(int order[], float cost);

/*
 * メイン関数
 */
int main()
{
    struct TSP tsp;
    srand((unsigned)time(NULL));

    ReadData(&tsp);

    // --- 前回の記録（コストと累計試行回数）を引き継ぐ ---
    FILE *f_prev = fopen("best_result.txt", "r");
    cost_min = 1e30; // 非常に大きな値で初期化
    trial_count = 0;

    if (f_prev != NULL) {
        char line[1024];
        while (fgets(line, sizeof(line), f_prev)) {
            // "Trial: 1254 (Cumulative)" のような形式から数値を読み取る
            if (sscanf(line, "Trial: %llu", &trial_count) == 1) {
                // 読み込み成功
            }
            if (sscanf(line, "Cost: %f", &cost_min) == 1) {
                // 読み込み成功
            }
        }
        fclose(f_prev);
        printf("Record loaded: Cost %.1f, Total Trials %llu\n", cost_min, trial_count);
    } else {
        printf("No previous record found. Starting from scratch.\n");
    }

    printf("Search started... (Press Ctrl+C to stop)\n");
    Infinite_Search(&tsp);

    return 0;
}
/*
 * 都市座標データ読み込み
 */
void ReadData(struct TSP *tsp)
{
    /* ファイル名を作成 */
    char filename[256];
    sprintf(filename, "cities2025_30-100/cities%d.csv", N);    

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Can't open data file: %s\n", filename);
        exit(1);
    }

    /* ファイル読み込み */
    char buf[256];
    for (int i = 0; i < N; i++) {
        if (fgets(buf, 256 - 1, fp) != NULL) {
            sscanf(buf, "%d,%d\n", &tsp->city[i].x, &tsp->city[i].y);
        }
    }
    fclose(fp);
}

/*
 * 初期巡回路を乱数で決定する
 */
void InitialOrder(struct TSP *tsp)
{
    int used[N] = {0};
    tsp->order[0] = 0;              // 最初の都市は固定
    used[0] = 1;

    for (int index = 1; index < N; index++) {
        while (1) {
            int tmp = rand() % N;
            if (used[tmp] == 0) {
                tsp->order[index] = tmp;
                used[tmp] = 1;
                break;
            }
        }
    }
}

/*
 * 2-opt法による局所解探索（最急降下法）
 */
void SDM(struct TSP *tsp)
{
    while (1) {
        float diff, max_diff = 0;
        int x1, x2, best_x1, best_x2;
        int changedOrder[N];

        float oldcost = tsp->cost;
        
        /* すべての x1, x2 をまわす */
        for (int i = 0; i <= N - 2; i++) {
            for (int j = i + 1; j <= N - 1; j++) {
                x1 = i;
                x2 = j;
                
                TwoOpt(tsp->order, changedOrder, x1, x2);
                float changedCost = CalcCostOrder(tsp, changedOrder);

                diff = oldcost - changedCost;

                if (diff > max_diff) {
                    max_diff = diff;
                    best_x1 = x1;
                    best_x2 = x2;
                }
            }
        }

        /* これ以上改善しなかったら break */
        if (max_diff <= 0)
            break;
        /* 改善する余地があるんだったら order を変える */
        else {
            UpdateOrder(tsp, best_x1, best_x2);
        }
    }
}

/*
 * 無限ループ探索関数
 * ベスト更新時にファイルを上書き保存する
 */
void Infinite_Search(struct TSP *tsp)
{
    while (1) {
        trial_count++; // 前回の続きからカウントアップ

        InitialOrder(tsp);
        CalcCost(tsp);
        SDM(tsp);
        
        if (tsp->cost < cost_min) {
            cost_min = tsp->cost;
            for (int i = 0; i < N; i++) {
                best_order[i] = tsp->order[i];
            }

            // ファイルに書き出し
            FILE *f_best = fopen("best_result.txt", "w");
            if (f_best != NULL) {
                fprintf(f_best, "Trial: %llu\n", trial_count);
                fprintf(f_best, "Cost: %.1f\n", cost_min);
                fprintf(f_best, "Path: ");
                for (int i = 0; i < N; i++) {
                    fprintf(f_best, "%d%s", best_order[i], (i == N - 1) ? "" : "-");
                }
                fprintf(f_best, "\n");
                fclose(f_best);
            }

            printf("[%llu] Update! New Best: %.1f\n", trial_count, cost_min);
            fflush(stdout);
        }
    }
}


void TwoOpt(const int currentOrder[N], int changedOrder[N], int x1, int x2)
{
    for (int i = 0; i < x1; i++) {
        changedOrder[i] = currentOrder[i];
    }
    int count = x2 - x1 + 1;
    for (int i = 0; i < count; i++) {
        changedOrder[i + x1] = currentOrder[x1 + count - 1 - i];
    }
    for (int i = x2 + 1; i < N; i++) {
        changedOrder[i] = currentOrder[i];
    }
}

int UpdateOrder(struct TSP *tsp, int x1, int x2)
{
    int changedOrder[N];
    TwoOpt(tsp->order, changedOrder, x1, x2);
    
    float current_cost = CalcCostOrder(tsp, tsp->order);
    float changed_cost = CalcCostOrder(tsp, changedOrder);

    if (changed_cost < current_cost) {
        for (int i = 0; i < N; i++) {
            tsp->order[i] = changedOrder[i];
        }
        tsp->cost = changed_cost;
        return 1;
    } else
        return 0;
}

void CalcCost(struct TSP *tsp)
{
    tsp->cost = 0;
    for (int i = 0; i < N - 1; i++) {
        tsp->cost += CalcDistance(tsp->city[tsp->order[i + 1]], tsp->city[tsp->order[i]]);
    }
    tsp->cost += CalcDistance(tsp->city[tsp->order[N - 1]], tsp->city[tsp->order[0]]);
}

float CalcCostOrder(struct TSP *tsp, int order[])
{
    float cost = 0;
    for (int i = 0; i < N - 1; i++) {
        cost += CalcDistance(tsp->city[order[i + 1]], tsp->city[order[i]]);
    }
    cost += CalcDistance(tsp->city[order[N - 1]], tsp->city[order[0]]);
    return cost;
}

float CalcDistance(struct City a, struct City b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

void ShowResult(int order[], float cost)
{
    printf("Shortest path and the cost：\n");
    for (int i = 0; i < N; i ++) {
        printf("C%-2d> ", order[i] + 1);
    }
    printf("C%-2d  cost =%7.1f\n", order[0] + 1, cost);
}