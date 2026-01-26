
/*
 * 医工学実験１ C言語プログラミング４
 * 巡回セールスマン問題を2-opt法で解く
 * 課題番号：1
 * 作成者：下地慶英
 * 作成日：2026/01/27
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifndef N			/* "gcc -DN=15 -lm *.c " で "#define N 15" とする */
#define N 30		/* 都市の数 */
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
int order_min[N];		/* 最短経路を保持するための配列 */

/* 関数の宣言 */
void ReadData(struct TSP *tsp);
void ShowData(struct TSP *tsp);
void InitialOrder(struct TSP *tsp);
void TwoOpt(const int currentOrder[N], int changedOrder[N], int x1, int x2);
void CalcCost(struct TSP *tsp);
float CalcDistance(struct City a, struct City b);
void ShowCost(struct TSP *tsp);
void ShowResult();

/*
* メイン関数
*/
int main()
{
	struct TSP tsp;
	
	srand((unsigned)time(NULL));	// rand() を初期化

	ReadData(&tsp);
	ShowData(&tsp);
	InitialOrder(&tsp);

	/* currentOrder に tsp.order をコピー */
	int currentOrder[N], changedOrder[N];
	for (int i = 0; i < N; i++) {
		currentOrder[i] = tsp.order[i];
	}

	/* x1 をユーザーに入力させる */
	int x1, x2;
	printf("x1 = ");	
	scanf("%d", &x1);
	if (x1 < 0 || N - 2 < x1) {
		printf("error: x1 must be between 0 and %d", N - 2);
		exit(1);
	}

	/* x2 をユーザーに入力させる */
	printf("x2 = ");	
	scanf("%d", &x2);
	if (x2 < 0 || N - 1 < x2) {
		printf("error: x2 must be between 0 and %d", N - 1);
		exit(1);
	} else if (x1 >= x2) {
		printf("error: x2 must be bigger than x1");
		exit(1);
	}

	/* currentOrder を表示 */
	printf("currentOrder: ");
	CalcCost(&tsp);
	ShowCost(&tsp);
	
	TwoOpt(currentOrder, changedOrder, x1, x2);
	
	/* changedOrder に tsp.order をコピー */
	for (int i = 0; i < N; i++) {
		tsp.order[i] = changedOrder[i] ;
	}
	printf("changedOrder: ");
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
 * 初期巡回路を乱数で決定する
 * 引数：struct TSP *tsp : TSPデータ
 */
void InitialOrder(struct TSP *tsp)
{
	printf("\nInitial order:\n");	 /* 計算始めの表示 */

	int used[N] = {0};
	tsp->order[0] = 0;				// 最初の都市は固定
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
 * 2-opt法による巡回路変形
 * 引数１：const int currentOrder[] : InitialOrder からコピーした元の順番(変更しない)
 * 引数２：int changedOrder[] : currentOrder を変形した巡回路を格納する配列 
 * 引数３：int x1 : １つめのパラメータ
 * 引数４：int x2 : ２つめのパラメータ
 */
void TwoOpt(const int currentOrder[N], int changedOrder[N], int x1, int x2)
{
	/* 0 番目から x1 - 1 番目までコピー */
	for (int i = 0; i < x1; i++) {
		changedOrder[i] = currentOrder[i];
	}

	/* x1 番目から x2 番目を逆順にコピー */
	int count = x2 - x1 + 1;
	for (int i = 0; i < count; i++) {
		changedOrder[i + x1] = currentOrder[x1 + count - 1 - i];
	}

	/* x2 +　1 番目から N - 1 番目までコピー */
	for (int i = x2 + 1; i < N; i++) {
		changedOrder[i] = currentOrder[i];
	}
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
	int i;

	for (i = 0; i < N; i ++) {
		printf("C%-2d> ", tsp->order[i] + 1);
	}
	printf("C%-2d  cost =%7.1f\n", tsp->order[0] + 1, tsp->cost);
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