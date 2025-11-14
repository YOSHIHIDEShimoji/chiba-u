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

#ifndef N			/* "gcc -DN=15 -lm *.c " で "#define 15" とする */
#define N 5			/* 都市の数 */
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

float cost_min;			/* 最短の移動距離 */
int first = 1;			/* cost_min に最初の経路の cost を代入するためのフラグ */
int order_min[N];		/* 最短経路を保持するための配列 */

/* 関数の宣言 */
void ReadData(struct TSP *tsp);
void ShowData(struct TSP *tsp);
float CalcDistance(struct City a, struct City b);
void SimpleOrder(struct TSP *tsp);
void AllOrder(struct TSP *tsp);
void CalcCost(struct TSP *tsp);
void ShowCost(struct TSP *tsp);
void CalcMin(struct TSP *tsp);
void ShowResult();

/*
 * メイン関数
 */
int main()
{
	struct TSP tsp;

	ReadData(&tsp);
	ShowData(&tsp);
	// SimpleOrder(&tsp);
	AllOrder(&tsp);
	// CalcCost(&tsp);
	// ShowCost(&tsp);
	ShowResult();
	return 0;
}

/*
 * 都市座標データ読み込み
 * 引数：struct TSP *tsp : TSPデータ
 */
void ReadData(struct TSP *tsp)
{
	/* 課題１で作成 */
	/* ファイル名を作成 */
	char filename[256];
	if (1 <= N && N <=9) {
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
		printf("C%-2d : %4d,%4d\n", 
			i + 1, tsp->city[i].x, tsp->city[i].y);
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
 * すべての巡回組み合わせを生成する(今回は N = 5)
 * 引数：struct TSP *tsp : TSPデータ
 */
void AllOrder(struct TSP *tsp)
{
	printf("\nAll order:\n"); /* 計算始めの表示 */

	tsp->order[0] = 0;		// 最初の都市は固定
	for (int i = 1; i < 5; i++) {
		tsp->order[1] = i;
		for (int j = 1; j < 5; j++) {
			if (j == i)
				continue;
			else {
				tsp->order[2] = j;
				for (int k = 1; k < 5; k++) {
					if (k == i || k == j)
						continue;
					else {
						tsp->order[3] = k;
						for (int l = 1; l < 5; l++) {
							if (l == i || l == j || l == k)
								continue;
							else {
								tsp->order[4] = l;
								break;
							}
						}
						CalcCost(tsp);
						ShowCost(tsp);
						CalcMin(tsp);
					}
				}
			}
		}
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

/*
 * 最初の経路の cost を cost_min に代入し、 order_min に tsp->order をコピーする
 * 引数：struct TSP *tsp : TSPデータ
 */
void CalcMin(struct TSP *tsp)
{
	if (first) {
		cost_min = tsp->cost;
		for (int i = 0; i < N; i++) {
			order_min[i] = tsp->order[i];
		}
		first = 0;
	} else if(cost_min >= tsp->cost) {
		cost_min = tsp->cost;
		for (int i = 0; i < N; i++) {
			order_min[i] = tsp->order[i];
		}
	}
}

/*
 * 最短経路とその距離を表示
 * 引数なし
 */
void ShowResult()
{
	printf("\nShortest path and cost：\n");
	for (int i = 0; i < N; i ++) {
		printf("C%-2d> ", order_min[i] + 1);
	}
	printf("C%-2d  cost =%7.1f\n", order_min[0] + 1, cost_min);
}