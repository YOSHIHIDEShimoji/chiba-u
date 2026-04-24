/* 課題名 pd03a じゃんけんの拡張
 * 氏名：下地慶英　学生証番号：24TB4039　日付：2026/04/23

 * 説明：
    ・既存の2人のじゃんけん場合のコードを参考にして、3人に拡張した。
    ・標準出力時にきれいに出力されるように改行の位置を工夫した。

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
int main(void) {
    int winPoint = 3;       /* 勝利に必要なポイント */
    int round  = 0;         /* 現在の対戦数 */
    int random = 0;         /* 乱数を記録 */
    int player1Hand = 0;    /* プレイヤー１の手 */
    int player2Hand = 0;    /* プレイヤー２の手 */
    int player3Hand = 0;    /* プレイヤー3の手 */
    int player1Point = 0;   /* プレイヤー１のポイント */
    int player2Point = 0;   /* プレイヤー２のポイント */
    int player3Point = 0;   /* プレイヤー３のポイント */

    /* 対戦前の初期化 */
    srand((unsigned int)time(NULL));    // 実行時の時間を使って乱数を初期化
    printf("じゃんけんを開始します\n");

    /* 対戦のループ */
    do {
        printf("\n%d回戦\n", ++round);

        /* プレイヤー１の手を決める */
        // G, C, P の順番で出す
        if (round % 3 == 0) {
            player1Hand = PAPER;
            printf("パー");
        } else if (round % 3 == 1) {
            player1Hand = STONE;
            printf("グー");
        } else {
            player1Hand = CHOKI;
            printf("チョキ");
        }
        printf(" vs ");
        
        /* プレイヤー２の手を決める */
        // G:P:C = 3:2:5 の確率
        random = rand() % 300; /* 0～299の乱数を発生 */
        if (random < 60) {
            player2Hand = PAPER;
            printf("パー");
        } else if (random < 60 + 90) {
            player2Hand = STONE;
            printf("グー");
        } else {
            player2Hand = CHOKI;
            printf("チョキ");
        }
        printf(" vs ");

        /* プレイヤー3の手を決める */
        // 最初の３回はグーでそれ以降はランダム
        if (round <= 3) {
            player3Hand = STONE;
            printf("グー");
        }  else {
            random = rand() % 300; /* 0～299の乱数を発生 */
            if (random < 100) {
                player3Hand = PAPER;
                printf("パー");
            } else if (random < 200) {
                player3Hand = STONE;
                printf("グー");
            } else {
                player3Hand = CHOKI;
                printf("チョキ");
            }
        }

        /* 勝敗判定 */
        // 0人勝ち（あいこ）
        if ((player1Hand == player2Hand && player2Hand == player3Hand) ||  // 3人とも一緒
            (player1Hand != player2Hand && player2Hand != player3Hand && player3Hand != player1Hand)) {   // 3人とも違う
            printf("\n引き分け");
        }   
        printf("\n");
        
        // 1人勝ち
        if ((player1Hand == PAPER && player2Hand == STONE && player3Hand == STONE) ||
            (player1Hand == STONE && player2Hand == CHOKI && player3Hand == CHOKI) ||
            (player1Hand == CHOKI && player2Hand == PAPER && player3Hand == PAPER)) {
            printf("プレイヤー１が勝ちました\n");
            player1Point++;
        }
        if ((player2Hand == PAPER && player3Hand == STONE && player1Hand == STONE) ||
            (player2Hand == STONE && player3Hand == CHOKI && player1Hand == CHOKI) ||
            (player2Hand == CHOKI && player3Hand == PAPER && player1Hand == PAPER)) {
            printf("プレイヤー２が勝ちました\n");
            player2Point++;
        }
        if ((player3Hand == PAPER && player1Hand == STONE && player2Hand == STONE) ||
            (player3Hand == STONE && player1Hand == CHOKI && player2Hand == CHOKI) ||
            (player3Hand == CHOKI && player1Hand == PAPER && player2Hand == PAPER)) {
            printf("プレイヤー３が勝ちました\n");
            player3Point++;
        }

        // 2人勝ち
        // player1 と player2
        if ((player1Hand == PAPER && player2Hand == PAPER && player3Hand == STONE) ||
            (player1Hand == STONE && player2Hand == STONE && player3Hand == CHOKI) ||
            (player1Hand == CHOKI && player2Hand == CHOKI && player3Hand == PAPER)) {
            printf("プレイヤー１とプレイヤー２が勝ちました\n");
            player1Point++;
            player2Point++;
        }
        // player2 と player3
        if ((player2Hand == PAPER && player3Hand == PAPER && player1Hand == STONE) ||
            (player2Hand == STONE && player3Hand == STONE && player1Hand == CHOKI) ||
            (player2Hand == CHOKI && player3Hand == CHOKI && player1Hand == PAPER)) {
            printf("プレイヤー２とプレイヤー３が勝ちました\n");
            player2Point++;
            player3Point++;
        }
        // player3 と player1
        if ((player3Hand == PAPER && player1Hand == PAPER && player2Hand == STONE) ||
            (player3Hand == STONE && player1Hand == STONE && player2Hand == CHOKI) ||
            (player3Hand == CHOKI && player1Hand == CHOKI && player2Hand == PAPER)) {
            printf("プレイヤー１とプレイヤー３が勝ちました\n");
            player3Point++;
            player1Point++;
        }
    } while(player1Point < winPoint && 
            player2Point < winPoint && 
            player3Point < winPoint); /* 終了判定 */

    /* 対戦結果の表示 */
    printf("\nじゃんけん終了\n");
    printf("ポイント %d 対 %d 対 %d で\n", player1Point, player2Point, player3Point);
    if (player1Point >= winPoint)
        printf("プレイヤー１の勝ちです\n");
    if (player2Point >= winPoint)
        printf("プレイヤー２の勝ちです\n");
    if (player3Point >= winPoint)
        printf("プレイヤー３の勝ちです\n");

    printf("\n");

    return 0;
}
