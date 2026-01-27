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

#ifndef N			/* "gcc -DN=100 -lm *.c " で "#define N 100" とする */
#define N 30		/* デフォルト都市の数 */
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

int best_order[N];		/* 最短経路を保持するための配列 */

/* 関数の宣言 */
void ReadData(struct TSP *tsp);
void ShowData(struct TSP *tsp);
void Greedy(struct TSP *tsp, int start);
void All_greedy(struct TSP *tsp);
void CalcCost(struct TSP *tsp);
float CalcDistance(struct City a, struct City b);
void ShowCost(struct TSP *tsp);
void ShowCostOrder(int order[], float cost);
void ShowResult(int order[], float cost);

/*
 * メイン関数
 */
int main()
{
	struct TSP tsp;
	
	ReadData(&tsp);
	ShowData(&tsp);

	printf("\nGreedy search:\n");
	All_greedy(&tsp);

	/* 最適解を表示 */
	ShowResult(tsp.order, tsp.cost);

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
	sprintf(filename, "cities2025_30-100/cities%d.csv", N);	

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
 * 欲張り法による巡回路生成
 * 引数１：struct TSP *tsp : TSPデータ（生成された巡回順が格納される）
 * 引数２：int start : 出発都市のインデックス
 */
void Greedy(struct TSP *tsp, int start)
{
	int is_used[N] = {0};
	tsp->order[0] = start;
	is_used[start] = 1;
	
	for (int index = 1; index < N; index++) {
		float best_cost;
		int best_city;
		int is_first = 1;
		
		for (int i = 0; i < N; i++) {
			if (is_used[i])
				continue;

			float dis = CalcDistance(tsp->city[tsp->order[index - 1]], tsp->city[i]);

			if (is_first || dis < best_cost) {
				best_cost = dis;
				best_city = i;
				is_first = 0;
			}
		}

		tsp->order[index] = best_city;
		is_used[best_city] = 1;
	}
}

/*
 * 全都市を開始地点とした欲張り法
 * 引数：struct TSP *tsp : TSPデータ
 */
void All_greedy(struct TSP *tsp)
{
	int is_first = 1;
	float best_cost;

	for (int start = 0; start < N; start++) {
		Greedy(tsp, start)	;

		CalcCost(tsp);
		ShowCost(tsp);	

		if (is_first || tsp->cost < best_cost) {
			best_cost = tsp->cost;
			for (int i = 0; i < N; i++) {
				best_order[i] = tsp->order[i];
			}
			is_first = 0;
		}
	}

	/* TSP に反映 */
	for (int i = 0; i < N; i++)	{
		tsp->order[i] = best_order[i];
	}
	tsp->cost = best_cost;
}

/*
 * 総移動距離を計算する
 * 引数：struct TSP *tsp : TSPデータ
 */
void CalcCost(struct TSP *tsp)
{
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
	float dis = sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	return dis;
}

/*
 * 巡回順と総移動距離を表示
 * 引数：struct TSP *tsp : TSPデータ
 */
void ShowCost(struct TSP *tsp)
{
	ShowCostOrder(tsp->order, tsp->cost);
}

/*
 * 巡回順と総移動距離を表示
 * 引数１：表示したい配列
 * 引数２：表示したい配列のコスト
 */
void ShowCostOrder(int order[], float cost)
{
	for (int i = 0; i < N; i ++) {
		printf("C%-2d> ", order[i] + 1);
	}
	printf("C%-2d  cost =%7.1f\n", order[0] + 1, cost);
}

/*
 * 最短経路とその距離を表示
 * 引数なし
 */
void ShowResult(int order[], float cost)
{
	printf("\nShortest path and the cost：\n");
	for (int i = 0; i < N; i ++) {
		printf("C%-2d> ", order[i] + 1);
	}
	printf("C%-2d  cost =%7.1f\n", order[0] + 1, cost);
}