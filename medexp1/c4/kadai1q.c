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

/*
 * メイン関数
 */
int main()
{
	struct TSP tsp;

	ReadData(&tsp);
	ShowData(&tsp);
	return 0;
}

/*
 * 都市座標データ読み込み
 * 引数：struct TSP *tsp : TSPデータ
 */
void ReadData(struct TSP *tsp)
{
	/* 課題1で作成する */
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
