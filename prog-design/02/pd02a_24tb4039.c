/* 課題名：pd02a 乗算の回文数
 * 氏名：下地慶英   学生証番号：24TB4039    日付：2026/04/16

 * 説明：以下の点について工夫を施した。
 * ・main 関数を少なく記述するために関数を分割し、それぞれわかり易い名前、コメントを心がけた。
 * ・calloc を用いて配列を動的確保する設計にした
 * ・各隙間に対して「乗算(True)」か「結合(False)」で表していき、
 * それをC言語の真偽値である「1と0」に置き換えてき出してみたところ、
 * その組み合わせが0から順に増える2進数の連番と一致することに気づきそれを基に設計した


 * 開発環境：Wandbox(online) https://wandbox.org/
 * コンパイラー：gcc 13.2.0
 * コマンド：gcc -Wall -Wextra -std=c11 -pedantic
 */

#include <stdio.h>
#include <stdlib.h>

int power(int a, int b);
void seekAnswers(int answers[], int min, int max);
int isAnswer(int value);
void showAnswers(int answers[]);


#ifndef N           // gcc -DN=8 [file_name].c で N = 8 に設定
#define N 6         // N のデフォルト値 6
#endif

int main(void) {
    
    // 総当り探索する最小値：min と最大値：max を求める
    int min = power(10, N - 1);
    int max = power(10, N) - 1;

    // 答えを answers[] に格納して表示
    int num_answers = power(10, N);
    int *answers = (int *)calloc(num_answers, sizeof(int));

    seekAnswers(answers, min, max);
    showAnswers(answers);

    free(answers);
    return 0;
}

// 累乗を求める関数
int power(int a, int b) {
    int result = 1;
    for (int i = 0; i < b; i++) {
        result *= a;
    }
    return result;
}

// 答えを求める関数
void seekAnswers(int answers[], int min, int max) {
    int index = 0;
    for (int i = min; i <= max; i++) {
        if (isAnswer(i)) {
            answers[index] = i;
            index++;
        }
    }
}

// 答えかどうか判定する関数
int isAnswer(int value) {

    // 各桁を配列の各要素に格納する
    int digits[N] = {0};
    int tmp = value;
    for (int i = N - 1; i >= 0; i--) {
        digits[i] = tmp % 10;
        tmp /= 10;
    }

    // value の逆順の数値：rev_num を求める
    int rev_num = 0;
    for (int i = N - 1; i >= 0; i--) {
        rev_num += digits[i] * power(10, i);
    }

    // 全パターンを計算する
    int gaps = N - 1;
    int patterns = power(2, gaps);

    for (int i = 1; i < patterns; i++) {

        int product = 1;
        int current = digits[0];

        // 隙間の数だけシフトさせて、掛け算 or 結合 かを判定
        for (int j = 0; j < gaps; j++) {
            int bit = (i >> (gaps - 1 - j)) & 1;
            if (bit == 0) {
                // 結合
                current = current * 10 + digits[j + 1];
            } else {
                // 掛け算
                product *= current;
                current = digits[j + 1];
            }
        }
        // 最後の塊と掛け算
        product *= current;

        if (product == rev_num)
            return 1;   

    }
    
    return 0;
}

// 答えを表示する関数
void showAnswers(int answers[]) {
    printf("All Answers:\n");
    int i = 0;
    for (i = 0; answers[i + 1] != 0; i++) {
        printf("%d, ", answers[i]);
    }
    printf("%d\n", answers[i]);
    
}
