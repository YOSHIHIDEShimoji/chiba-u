/* 課題： pd04a Nクイーン問題ソルバー
 * 氏名：下地慶英　学生証番号：24TB4039　日付：2026/05/19
 * 説明：
 *   ・サンプルの4重ネストループを再帰バックトラッキングに置き換え、任意のN（N >= 4）に対応させた
 *   ・サンプルの4x4固定だった対角チェックを、全クイーンペアの差分を比較する方式に変更した
 * 開発環境：Wandbox(online) https://wandbox.org/
 * コンパイラー：gcc 13.2.0
 * コマンド：gcc -Wall -Wextra -std=c11 -pedantic
 */

#include <stdio.h>
#include <stdlib.h>

#define MINIMUM_QUEENS 4
#define SET 1
#define NONE 0

typedef unsigned char BYTE;

/* Show queens board on display */
void showBoard(BYTE *board, int n) {
    static int count = 0;
    printf("Soluton %d of %d-Queens problem\n", ++count, n);
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            int idx = row * n + col;
            printf("%s", board[idx] == SET ? "QQ" : "  ");
            if (col < n - 1) printf("|");
        }
        printf("\n");
        if (row < n - 1) {
            for (int col = 0; col < n; col++) {
                printf("%s", col < n - 1 ? "--+" : "--");
            }
            printf("\n");
        }
    }
    printf("\n");
}

/* Judge the board return 1:OK 0:NG */
int checkBoard(BYTE *board, int n) {
    int count;

    /* check horizontal lines */
    for (int row = 0; row < n; row++) {
        count = 0;
        for (int col = 0; col < n; col++) {
            count += board[row * n + col];
        }
        if (count != 1) return 0;
    }

    /* check vertical lines */
    for (int col = 0; col < n; col++) {
        count = 0;
        for (int row = 0; row < n; row++) {
            count += board[row * n + col];
        }
        if (count != 1) return 0;
    }

    /* check diagonal lines */
    for (int r1 = 0; r1 < n; r1++) {
        for (int c1 = 0; c1 < n; c1++) {
            if (board[r1 * n + c1] != SET) continue;
            for (int r2 = r1 + 1; r2 < n; r2++) {
                for (int c2 = 0; c2 < n; c2++) {
                    if (board[r2 * n + c2] != SET) continue;
                    if (r1 - r2 == c1 - c2 || r1 - r2 == c2 - c1) return 0;
                }
            }
        }
    }

    return 1;
}

/* recursive search */
void solve(BYTE *board, int n, int col) {
    if (col == n) {
        if (checkBoard(board, n)) showBoard(board, n);
        return;
    }
    for (int row = 0; row < n; row++) {
        board[row * n + col] = SET;
        solve(board, n, col + 1);
        board[row * n + col] = NONE;
    }
}

/* N-Queens solver */
void nqueens(int n) {
    BYTE *board = (BYTE *)malloc(sizeof(BYTE) * n * n);
    for (int i = 0; i < n * n; i++) {
        board[i] = NONE;
    }

    solve(board, n, 0);

    free(board);
}

/* Start point */
int main(void) {
    /* input N */ 
    int n = 0;
    printf("How many queens?");
    scanf("%d", &n);
    printf("\n\n");

    if (n < MINIMUM_QUEENS) {
        printf("number of queens must be %d or more.\n", MINIMUM_QUEENS);
        return 1;
    }

    nqueens(n);

    return 0;
}
