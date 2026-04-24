/* 課題名 pd03a じゃんけんの拡張
 * 氏名：　　　学生証番号：　　　日付：2026/  /
 * 説明：
 * 開発環境：Wandbox(online) https://wandbox.org/
 * コンパイラー：gcc 13.2.0
 * コマンド：gcc -Wall -Wextra -std=c11 -pedantic
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>       // 乱数の初期化にtime関数を使用

#define PAPER 1
#define STONE 2
#define CHOKI 3

/* メイン関数
 */
int main() {
    int winPoint = 3;       /* 勝利に必要なポイント */
    int round  = 0;         /* 現在の対戦数 */
    int random = 0;         /* 乱数を記録 */
    int player1Hand = 0;    /* プレイヤー１の手 */
    int player2Hand = 0;    /* プレイヤー２の手 */
    int player1Point = 0;   /* プレイヤー１のポイント */
    int player2Point = 0;   /* プレイヤー２のポイント */

    /* 対戦前の初期化 */
    srand((unsigned int)time(NULL));    // 実行時の時間を使って乱数を初期化
    printf("じゃんけんを開始します\n\n");

    /* 対戦のループ */
    do {
        printf("%d回戦\n", ++ round);

        /* プレイヤー１の手を決める */
        random = rand() % 300; /* 0～299の乱数を発生 */
        if (random < 100) {
            player1Hand = PAPER;
            printf("パー");
        } else if (random < 200) {
            player1Hand = STONE;
            printf("グー");
        } else {
            player1Hand = CHOKI;
            printf("チョキ");
        }
        printf(" vs ");

        /* プレイヤー２の手を決める */
        random = rand() % 300; /* 0～299の乱数を発生 */
        if (random < 100) {
            player2Hand = PAPER;
            printf("パー");
        } else if (random < 200) {
            player2Hand = STONE;
            printf("グー");
        } else {
            player2Hand = CHOKI;
            printf("チョキ");
        }
        printf("\n");

        /* 勝敗判定 */
        if ((player1Hand == PAPER && player2Hand == STONE) ||
            (player1Hand == STONE && player2Hand == CHOKI) ||
            (player1Hand == CHOKI && player2Hand == PAPER)) {
            printf("プレイヤー１が勝ちました\n");
            player1Point ++;
        }
        if ((player2Hand == PAPER && player1Hand == STONE) ||
            (player2Hand == STONE && player1Hand == CHOKI) ||
            (player2Hand == CHOKI && player1Hand == PAPER)) {
            printf("プレイヤー２が勝ちました\n");
            player2Point ++;
        }
        if (player1Hand == player2Hand) {
            printf("引き分け\n");
        }
        printf("\n");
    } while(player1Point < winPoint && 
            player2Point < winPoint); /* 終了判定 */

    /* 対戦結果の表示 */
    printf("じゃんけん終了\n");
    printf("ポイント %d 対 %d で", player1Point, player2Point);
    if (player1Point >= winPoint) {
        printf("プレイヤー１の勝ちです\n");
    } else {
        printf("プレイヤー２の勝ちです\n");
    }
    printf("\n");

    return 0;
}
