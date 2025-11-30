
/*
 * 医工学実験１ C言語プログラミング４
 * 巡回セールスマン問題を2-opt法で解く
 * 課題番号：
 * 作成者：
 * 作成日：
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
int first = 1;			/* cost_min に最初の経路の cost を代入するためのフラグ */
int order_min[N];		/* 最短経路を保持するための配列 */

/* 関数の宣言 */
void ReadData(struct TSP *tsp);
void ShowData(struct TSP *tsp);
void InitialOrder(struct TSP *tsp);
void SteepestDescentMethod(struct TSP *tsp);
void CalcCost(struct TSP *tsp);
float CalcDistance(struct City a, struct City b);
float CalcCostOrder(struct TSP *tsp, int order[]);
void ShowCost(struct TSP *tsp);
void ShowCostOrder(int order[], float cost);
void TwoOpt(const int currentOrder[N], int changedOrder[N], int x1, int x2);
int UpdateOrder(struct TSP *tsp, int x1, int x2);
void AllOrder(struct TSP *tsp, int index);
void CalcMin(struct TSP *tsp);
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

	/* currentOrder のコストを表示 */
	CalcCost(&tsp);
	ShowCost(&tsp);

	SteepestDescentMethod(&tsp);
	// while (1) {
	// 	float diff, max_diff = 0;
	// 	int x1, x2, best_x1, best_x2;
	// 	int changedOrder[N];

	// 	float oldcost = tsp.cost;
		
	// 	/* すべての x1, x2 をまわす */
	// 	for (int i = 0; i <= N - 2; i++) {
	// 		for (int j = i + 1; j <= N - 1; j++) {
	// 			x1 = i;
	// 			x2 = j;
				
	// 			TwoOpt(tsp.order, changedOrder, x1, x2);
	// 			float changedCost = CalcCostOrder(&tsp, changedOrder);

	// 			diff = oldcost - changedCost;

	// 			if (diff > max_diff) {
	// 				max_diff = diff;
	// 				best_x1 = x1;
	// 				best_x2 = x2;
	// 			}
	// 		}
	// 	}

	// 	/* これ以上改善しなかったら break */
	// 	if (max_diff <= 0)
	// 		break;
	// 	/* 改善する余地があるんだったら order を変える */
	// 	else {
	// 		UpdateOrder(&tsp, best_x1, best_x2);
	// 		ShowCost(&tsp);
	// 	}
	// }	
	
	// printf("\nAll order:\n"); 	/* 計算始めの表示 */
	// AllOrder(&tsp, 1);
	// CalcCost(&tsp);
	// ShowCost(&tsp);
	// ShowResult();
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
	sprintf(filename, "cities2024_30-100/cities%d.csv", N);	

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
 * 初期巡回路を乱数で決定する
 * 引数：struct TSP *tsp : TSPデータ
 */
void InitialOrder(struct TSP *tsp)
{
	/* 課題１で作成 */
	// printf("\nInitial order:\n");	 /* 計算始めの表示 */
	printf("\n");

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

void SteepestDescentMethod(struct TSP *tsp)
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
			ShowCost(tsp);
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

int UpdateOrder(struct TSP *tsp, int x1, int x2)
{
	int changedOrder[N];
	TwoOpt(tsp->order, changedOrder, x1, x2);
	
	/* currentOrder と changedOrder のコストを計算 */
	float current_cost = CalcCostOrder(tsp, tsp->order);
	float changed_cost = CalcCostOrder(tsp, changedOrder);

	/* cost の比較 */
	if (changed_cost < current_cost) {
		for (int i = 0; i < N; i++) {
			tsp->order[i] = changedOrder[i];
		}
		tsp->cost = changed_cost;
		return 1;
	} else
		return 0;
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
 * 配列から総移動距離を計算する
 * 引数１：struct TSP *tsp : TSPデータ
 * 引数２：計算したい配列
 */
float CalcCostOrder(struct TSP *tsp, int order[])
{
	float cost = 0;
 
	for (int i = 0; i < N - 1; i++) {
		cost += CalcDistance(tsp->city[order[i + 1]], tsp->city[order[i]]);
	}
	cost += CalcDistance(tsp->city[order[N - 1]], tsp->city[order[0]]);

	return cost;
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
 * 巡回順と総移動距離を表示
 * 引数１：表示したい配列
 * 引数２：表示したい配列のコスト
 */
void ShowCostOrder(int order[], float cost)
{
	int i;

	for (i = 0; i < N; i ++) {
		printf("C%-2d> ", order[i] + 1);
	}
	printf("C%-2d  cost =%7.1f\n", order[0] + 1, cost);
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
	printf("\nShortest path and cost：\n");
	for (int i = 0; i < N; i ++) {
		printf("C%-2d> ", order_min[i] + 1);
	}
	printf("C%-2d  cost =%7.1f\n", order_min[0] + 1, cost_min);
}