/* 課題： pd04a Nクイーン問題ソルバー
 * 氏名：　　　学生証番号：　　　日付：2026/  /
 * 説明：
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
void showBoard(BYTE* board, int n) {
    static int count = 0;
    int col, row;
    printf("Soluton %d of %d-Queens problem\n", ++count, n);
    for (row = 0; row < n; row ++){
        for (col = 0; col < n; col ++){
            int idx = row * n + col;
            printf("%s", board[idx] == SET ? "QQ" : "  ");
            if (col < n - 1) printf("|");
        }
        printf("\n");
        if (row < n - 1) {
            for (col = 0; col < n; col ++) {
                printf("%s", col < n - 1 ? "--+" : "--");
            }
            printf("\n");
        }
    }
    printf("\n");
}

/* Judge the board return 1:OK 0:NG */
int CheckBoard(BYTE* board, int n) {
    int row, col, count;

    /* check horizontal lines */
    for (row = 0; row < n; row ++) {
        count = 0;
        for (col = 0; col < n; col ++) {
            count += board[row * n + col];
        }
        if (count != 1) return 0;
    }

    /* check vertical lines */
    for (col = 0; col < n; col ++) {
        count = 0;
        for (row = 0; row < n; row ++) {
            count += board[row * n + col];
        }
        if (count != 1) return 0;
    }

    /* check cross lines */
    if (board[8] + board[13] > 1) return 0;
    if (board[4] + board[9] + board[14] > 1) return 0;
    if (board[0] + board[5] + board[10] + board[15] > 1) return 0;
    if (board[1] + board[6] + board[11] > 1) return 0;
    if (board[2] + board[7] > 1) return 0;

    if (board[1] + board[4] > 1) return 0;
    if (board[2] + board[5] + board[8] > 1) return 0;
    if (board[3] + board[6] + board[9] + board[12] > 1) return 0;
    if (board[7] + board[10] + board[13] > 1) return 0;
    if (board[11] + board[14] > 1) return 0;

    return 1;
}

/* N-Queens solver */
void nqueens(int n) {
    /* initialize board */
    BYTE* board = (BYTE*)malloc(sizeof(BYTE) * n * n);
    int i;
    for (i = 0; i < n * n; i ++){
        board[i] = NONE;
    }

    /* search (only for 4-queens) */
    int queen0row, queen1row, queen2row, queen3row;
    for (queen0row = 0; queen0row < n; queen0row ++){
        int queen0col = 0;
        int idx0 = queen0row * n + queen0col;
        board[idx0] = SET;

        for (queen1row = 0; queen1row < n; queen1row ++){
            int queen1col = 1;
            int idx1 = queen1row * n + queen1col;
            board[idx1] = SET;

            for (queen2row = 0; queen2row < n; queen2row ++){
                int queen2col = 2;
                int idx2 = queen2row * n + queen2col;
                board[idx2] = SET;

                for (queen3row = 0; queen3row < n; queen3row ++){
                    int queen3col = 3;
                    int idx3 = queen3row * n + queen3col;
                    board[idx3] = SET;

                    // Check condition and show solution
                    if (CheckBoard(board, n) == 1) showBoard(board, n);

                    board[idx3] = NONE;
                }
                board[idx2] = NONE;
            }
            board[idx1] = NONE;
        }
        board[idx0] = NONE;
    }
 
    free(board);
}

/* Start point */
int main() {
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
