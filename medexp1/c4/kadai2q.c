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

#define N 10		/* 都市の数 */

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

/*
 * メイン関数
 */
int main()
{
	struct TSP tsp;

	ReadData(&tsp);
	ShowData(&tsp);

	/* 課題2で作成する：　総移動距離 */
	float distance = 0;
	for (int i = 0; i < N - 1; i++) {
		distance += sqrt((tsp.city[i + 1].x - tsp.city[i].x) * (tsp.city[i + 1].x - tsp.city[i].x)
					   + (tsp.city[i + 1].y - tsp.city[i].y) * (tsp.city[i + 1].y - tsp.city[i].y));
	}
	printf("%f\n", distance);
	return 0;
}

/*
 * 都市座標データ読み込み
 * 引数：struct TSP *tsp : TSPデータ
 */
void ReadData(struct TSP *tsp)
{
	/* 課題1で作成する */
    FILE *fp = fopen("cities2025_4-20/cities10.csv", "r");
    if (fp == NULL) {
        printf("Can't open data file.\n");
        exit(1);
    }

    char buf[256];
    for (int i = 0; i < N; i++) {
        fgets(buf, 256 - 1, fp);
        sscanf(buf, "%d,%d\n", &tsp->city[i].x, &tsp->city[i].y);
    }
	
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
