/*
 * 医工学実験１ C言語プログラミング４
 * 巡回セールスマン問題を総当たり法で解く
 * 課題番号：5
 * 作成者：下地慶英
 * 作成日：2026/01/20
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef N			/* "gcc -DN=15 -lm kadai5q.c" で "#define 15" とする */
#define N 10		/* デフォルトの都市の数 10 */
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
int is_first = 1;		/* cost_min に cost の最初の経路を代入するためのフラグ */
int order_min[N];		/* 最短経路を保持するための配列 */

/* 関数の宣言 */
void ReadData(struct TSP *tsp);
void ShowData(struct TSP *tsp);
float CalcDistance(struct City a, struct City b);
void SimpleOrder(struct TSP *tsp);
void AllOrder(struct TSP *tsp, int index);
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

	/* 計算始めの表示 */
	printf("\nAll order:\n");
	tsp.order[0] = 0;			// 最初の都市は固定
	AllOrder(&tsp, 1);

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
	/* データ表示 */
	printf("Cities location:\n");
	for (int i = 0; i < N; i ++) {
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
	
	/* 計算始めの表示 */
	printf("\nSimple order:\n");

	/* 課題３で作成 */
	for (int i = 0; i < N; i++) {
		tsp->order[i] = i;
	}
}

/*
 * すべての巡回組み合わせを生成する
 * 引数1：struct TSP *tsp : TSPデータ
 * 引数2：int index：tsp->order[index] を決める
 */
void AllOrder(struct TSP *tsp, int index)
{
	/* order が完成したら計算する */
	if (index == N) {
		CalcCost(tsp);
		ShowCost(tsp);
		CalcMin(tsp);
		return;
	}

	for (int i = 1; i < N; i++)	{

		/* index より前の配列番号に i が使われているかの判定 */
		int used = 0;
		for (int j = 1; j < index; j++) {
			if (tsp->order[j] == i) {
				used = 1;
				break;
			}
		}

		/* 使われていなかったら代入 */
		if (used)
			continue;
		else {
			tsp->order[index] =  i;
			AllOrder(tsp, index + 1);		
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
	for (int i = 0; i < N; i ++) {
		printf("C%-2d> ", tsp->order[i] + 1);
	}
	printf("C%-2d  cost =%7.1f\n", tsp->order[0] + 1, tsp->cost);
}

/*
 * tsp->order の最小値を order_min に代入する
 * 引数：struct TSP *tsp : TSPデータ
 */
void CalcMin(struct TSP *tsp)
{
	if (is_first) {
		cost_min = tsp->cost;
		for (int i = 0; i < N; i++) {
			order_min[i] = tsp->order[i];
		}
		is_first = 0;
	} else if (cost_min > tsp->cost) {
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
	printf("\nShortest path and the cost：\n");
	for (int i = 0; i < N; i ++) {
		printf("C%-2d> ", order_min[i] + 1);
	}
	printf("C%-2d  cost =%7.1f\n", order_min[0] + 1, cost_min);
}