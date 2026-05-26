/* 課題：pd05a C中心の文字列
 * 氏名：下地慶英   学生証番号：24TB4039    日付：2026/05/26
 *
 * 説明：・文字列中のC中心の奇数長部分文字列の個数を求めた
 *      ・カウントがintの範囲を超えるためlong longを使った
 *      ・入力末尾に付加される \n を \0 に置換し、'C'のカウント誤差を防止した
 *
 * 開発環境：Wandbox(online) https://wandbox.org/
 * コンパイラー：gcc 13.2.0
 * コマンド：gcc -Wall -Wextra -std=c11 -pedantic
 */

#include <stdio.h>
#include <string.h>

#define MAX_NUM_STRING 500001

int main(void) {
    char inputString[MAX_NUM_STRING];
    printf("input string: ");
    if (fgets(inputString, MAX_NUM_STRING, stdin) == NULL) {
        return 1;
    }
    int len = (int)strlen(inputString);
    if (len > 0 && inputString[len - 1] == '\n') {
        inputString[--len] = '\0';
    }

    long long count = 0;
    for (int i = 0; i < len; i++) {
        if (inputString[i] == 'C') {
            int j;
            if (i < len - 1 - i) {
                j = i;
            } else {
                j = len - 1 - i;
            }
            count += j + 1;
        }
    }

    printf("%lld\n", count);
    return 0;
}
