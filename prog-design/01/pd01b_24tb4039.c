/* 課題名 pd01b ワードカウント
 * 氏名：下地慶英　学生証番号：24TB4039　日付：2026/04/09

 * 説明：以下の点において可読性を向上させた
 * ・改行コードが LF だと思っていたが CRLF だったため、\r の時にスキップする処理を加えた
 * ・ファイルの末尾に \n がない場合、最終行がカウントされないため、ループ後に処理を追加した
 * ・最初はスペースの数で単語数をカウントしようとしたが、ハイフン等に対応できないため in_word フラグを用い、単語の始まりを検知する方式に変更した
 * ・ctype.h をインクルードすれば isalpha() でアルファベット判定ができるが、今回は範囲比較で実装した
 * 
 * 開発環境：Wandbox(online) https://wandbox.org/
 * コンパイラー：gcc 13.2.0
 * コマンド：gcc -Wall -Wextra -std=c11 -pedantic
 */

#include <stdio.h>
#include <stdlib.h>

// メイン関数
int main(void) {

    // ファイルを開く
    FILE *fp = fopen("./seton.txt", "r");
    if (fp == NULL) {
        printf("ファイルを開けません.\n");
        return 1;
    }

    // カウント用変数
    int c;                  // 代入用変数
    int sentences = 0;      // 文の数（ピリオドで終わる文章の数）
    int lines = 0;          // 行数（空行はカウントしない）
    int words = 0;          // 単語数
    int letters = 0;        // 文字数（スペース、改行はカウントしない）

    // 制御用変数
    int pre = '\n';         // 次のループ時に直前(previous)の文字に相当する文字を保存するための変数
    int in_word = 0;        // 代入用変数 c に入っている文字は単語内の文字かを判定するための変数

    while ((c = fgetc(fp)) != EOF) {

        // 改行コードが CRLF のときにも対応
        if (c == '\r')
            continue;

        // 文の数
        if (c == '.')
            sentences++;

        // 行数
        
        // 直前の文字が改行 \n の場合は空行と判定できる
        if (c == '\n' && pre != '\n')
            lines++;
        
        // 単語数
        if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) {

            // 単語内でなかったらカウントアップ
            if (!in_word) {
                words++;
                in_word = 1;
            }

        // シングルクォート ' は単語内
        } else if (c == '\'') {
            in_word = 1;
        
        // それ以外の文字（-, \n, スペースなど）は単語外
        } else {
            in_word = 0;
        }

        // 文字数
        if (c != '\n' && c != ' ')
            letters++;

        // 直前の文字を更新
        pre = c;

    }

    // 文の最後が改行 \n 以外で終わっている場合、1行追加されていない
    if (pre != '\n')
        lines++;

    // 出力
    printf("文の数：%d\n", sentences);
    printf("行数：%d\n", lines);
    printf("単語数：%d\n", words);
    printf("文字数：%d\n", letters);
}
