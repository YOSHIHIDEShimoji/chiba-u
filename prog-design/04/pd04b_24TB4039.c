/* 課題： pd04b 過剰数
 * 氏名：下地慶英　学生証番号：24TB4039　日付：2026/05/19
 * 説明：
 *   ・約数の探索をi <= n/2までに限定し、全数探索（i < n）より計算量を半減させた
 *   ・入力形式（カンマ区切り）を標準出力で案内してから入力を受け付けるようにした
 *   ・lower > upperの場合はエラーメッセージを出力して終了するようにした
 * 開発環境：Wandbox(online) https://wandbox.org/
 * コンパイラー：gcc 13.2.0
 * コマンド：gcc -Wall -Wextra -std=c11 -pedantic
 */

#include <stdio.h>

/* n自身を除く約数の和 */
int divisorSum(int n) {
    int sum = 1;
    for (int i = 2; i <= n / 2; i++) {
        if (n % i == 0) sum += i;
    }
    return sum;
}

int main(void) {
    int lower, upper;
    printf("Input range (lower,upper): ");
    scanf("%d,%d", &lower, &upper);

    if (lower > upper) {
        printf("Error: upper must be bigger than lower.\n");
        return 1;
    }

    for (int n = lower; n <= upper; n++) {
        int sum = divisorSum(n);
        if (sum > n) {
            printf("%d,%d\n", n, sum - n);
        }
    }

    return 0;
}
