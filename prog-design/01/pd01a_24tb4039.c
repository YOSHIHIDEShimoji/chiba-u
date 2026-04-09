/* 課題名 pd01a 平均値の計算（リファクタリング）
 * 氏名：下地慶英　学生証番号：24TB4039　日付：2026/04/09

 * 説明：以下の点において可読性を向上させた
 * ・割る数が 20 というバグを修正し、引数の n で割るように変更
 * ・表示処理を main() へ移動し、計算結果を戻り値で返す仕様に変更 
 * ・変数名を直感的に分かる sum, ave に変更
 * ・添字を一般的な i に変更し、forループ内で宣言するように変更
 * 
 * 開発環境：Wandbox(online) https://wandbox.org/
 * コンパイラー：gcc 13.2.0
 * コマンド：gcc -Wall -Wextra -std=c11 -pedantic
 */

#include <stdio.h>

// 平均を求めたい数の個数
#define N 6

// プロトタイプ宣言
double average(int a[], int n);

// メイン関数
int main(void) {
    int vals[N] = {5, 0, 3, 9, 2, 7};

    printf("numbers: ");
    for (int i = 0; i < N; i++) {
        printf("%d ", vals[i]);
    }
    printf("\n");

    double ave = average(vals, N);
    printf("average = %.2f\n", ave);

    return 0;
}

// 平均を求める関数
double average(int a[], int n)
{
    int sum = 0;
    for(int i = 0; i < n; i++) {
        sum = sum + a[i];
    }

    double ave = (double)sum / n;

    return ave;
}
