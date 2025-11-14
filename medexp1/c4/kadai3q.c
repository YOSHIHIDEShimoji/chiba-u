/*
 * 医工学実験１ C言語プログラミング４
 * 巡回セールスマン問題を総当たり法で解く
 * 課題番号：
 * 作成者：
 * 作成日：
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef N			/* "-DN=15" で "#define 15" とする */
#define N 10		/* 都市の数 */
#endif

/* 都市の座標構造体 */
struct City {
	int x, y;
};

/* 問題情報構造体 */
struct TSP {
	struct City city[N];	/* 都市の位置 */
	int order[N];			/* 巡回順 */
	float cost;				/* 総移動距離 */
};

/* 関数の宣言 */
void ReadData(struct TSP *tsp);
void ShowData(struct TSP *tsp);
float CalcDistance(struct City a, struct City b);
void SimpleOrder(struct TSP *tsp);
void CalcCost(struct TSP *tsp);
void ShowCost(struct TSP *tsp);

/*
 * メイン関数
 */
int main()
{
	struct TSP tsp;

	ReadData(&tsp);
	ShowData(&tsp);
	SimpleOrder(&tsp);
	CalcCost(&tsp);
	ShowCost(&tsp);

	return 0;
}

/*
 * 都市座標データ読み込み
 * 引数：struct TSP *tsp : TSPデータ
 */
void ReadData(struct TSP *tsp)
{
	/* ファイル名を作成 */
	char filename[256];
	if (4 <= N && N <=9) {
		sprintf(filename, "cities2025_4-20/cities0%d.csv", N);
	} else {
		sprintf(filename, "cities2025_4-20/cities%d.csv", N);
	}

	FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Can't open data file.\n");
        exit(1);
    }
	/* ファイル読み込み */
    char buf[256];
    for (int i = 0; i < N; i++) {
        fgets(buf, 256 - 1, fp);
        sscanf(buf, "%d,%d\n", &tsp->city[i].x, &tsp->city[i].y);
    }
	fclose(fp);
}

/*
 * 都市座標データ表示
 * 引数：struct TSP *tsp : TSPデータ
 */
void ShowData(struct TSP *tsp)
{
	int i;

	/* データ表示 */
	printf("Cities location:\n");
	for (i = 0; i < N; i ++) {
		printf("C%-2d : %4d,%4d\n", i + 1, tsp->city[i].x, tsp->city[i].y);
	}
}

/*
 * 番号順に巡回する
 * 引数：struct TSP *tsp : TSPデータ
 */
void SimpleOrder(struct TSP *tsp)
{
	printf("\nSimple order:\n"); /* 計算始めの表示 */

	/* 課題３で作成 */
	for (int i = 0; i < N; i++) {
		tsp->order[i] = i;
	}
}

/*
 * 総移動距離を計算する
 * 引数：struct TSP *tsp : TSPデータ
 */
void CalcCost(struct TSP *tsp)
{
	/* 課題3で作成 */
	/* 計算した総移動距離は tsp->cost に代入する */
	tsp->cost = 0;
	for (int i = 0; i < N - 1; i++) {
		tsp->cost += CalcDistance(tsp->city[tsp->order[i + 1]], tsp->city[tsp->order[i]]);
	}
	tsp->cost += CalcDistance(tsp->city[tsp->order[N - 1]], tsp->city[tsp->order[0]]);
}

/*
 * ２都市間の距離を計算
 * 引数：struct City a : 都市1
 * 引数：struct City b : 都市2
 * 戻り値：距離
 */
float CalcDistance(struct City a, struct City b)
{
	/* 課題3で作成 */
	float dis = sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	return dis;
}

/*
 * 巡回順と総移動距離を表示
 * 引数：struct TSP *tsp : TSPデータ
 */
void ShowCost(struct TSP *tsp)
{
	int i;

	for (i = 0; i < N; i ++) {
		printf("C%-2d> ", tsp->order[i] + 1);
	}
	printf("C%-2d  cost =%7.1f\n", tsp->order[0] + 1, tsp->cost);
}
