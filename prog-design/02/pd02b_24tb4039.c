/* 課題名：pd02b 単語出現頻度の集計
 * 氏名：下地慶英   学生証番号：24TB4039    日付：2026/04/18
 *
 * 説明：
 * 1. 単語と出現回数をセットで管理したかったが、Pythonの辞書型に相当する
 *  　データ構造がCには存在しないため、構造体WordInfoを自作して対応した。
 *　  WordInfoの配列を辞書として使い、単語の検索・登録・カウントを行う。
 * 
 * 2. 文字が単語を構成するか判定する関数 isWordChar で、if 分を使って処理してもよいが、
 *    以下のように書くと少なく書くことができた
 *      return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
 * 
 * 3. const は今まで使ったことなかったが、関数内で書き換えてはならないときに
 *    constを使い、ミスを防止した
 * 
 *
 * 開発環境：Wandbox(online) https://wandbox.org/
 * コンパイラー：gcc 13.2.0
 * コマンド：gcc -Wall -Wextra -std=c11 -pedantic
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_CHAR 100

// 単語と単語数を格納する構造体
typedef struct {
    char word[MAX_CHAR];
    int count;
} WordInfo;

// プロトタイプ宣言
int isWordChar(int c);
int toLower(int c);
int readWord(FILE *fp, char *word);
int isEqual(const char *a, const char *b);
int findWord(WordInfo *dict, int dict_size, const char *word);
void registerWord(WordInfo *dict, int *dict_size, const char *word);
void sortDict(WordInfo *dict, int dict_size);

// メイン関数
int main(void) {

    // ファイルを開く
    FILE *fp = fopen("seton2.txt", "r");
    if (fp == NULL) {
        printf("Can't open file.\n");
        exit(1);
    }

    // ファイルのバイト数をカウント
    int char_count = 0;
    while (fgetc(fp) != EOF) {
        char_count++;
    }

    // バイト数より単語数は少ないので、バイト数分確保しておく
    WordInfo *dict = (WordInfo *)malloc(char_count * sizeof(WordInfo));
    if (dict == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    fseek(fp, 0, SEEK_SET);

    // 単語を読み込んで辞書に登録
    char word[MAX_CHAR];
    int dict_size = 0;
    while (readWord(fp, word)) {
        registerWord(dict, &dict_size, word);
    }
    fclose(fp);

    // Nを入力
    int n;
    printf("N: ");
    scanf("%d", &n);

    // ソートして出力
    sortDict(dict, dict_size);
    for (int i = 0; i < dict_size; i++) {
        if (dict[i].count >= n) {
            printf("%s,%d\n", dict[i].word, dict[i].count);
        }
    }

    free(dict);
    return 0;
}

// 文字が単語を構成するか判定する関数
int isWordChar(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// 大文字を小文字に変換する関数
int toLower(int c) {
    if ('A' <= c && c <= 'Z')
        return c - 'A' + 'a';

    return c;
}

// ファイルから1単語読み取る関数
int readWord(FILE *fp, char *word) {
    int c;
    int i = 0;

    // 単語の始まりまでスキップ
    while ((c = fgetc(fp)) != EOF) {
        if (isWordChar(c)) {
            word[i++] = toLower(c);
            break;
        }
    }
    if (c == EOF) return 0;

    // 単語の残りを読む
    while ((c = fgetc(fp)) != EOF) {
        if (!isWordChar(c)) 
            break;

        if (i < MAX_CHAR - 1)
            word[i++] = toLower(c);
    }
    word[i] = '\0';
    return 1;
}

// 2つの文字列が等しいか比較する関数
int isEqual(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b)
            return 0;

        a++; b++;
    }
    return *a == *b;
}

// 辞書から単語を検索してインデックスを返す関数
int findWord(WordInfo *dict, int dict_size, const char *word) {
    for (int i = 0; i < dict_size; i++) {
        if (isEqual(dict[i].word, word)) 
            return i;
    }
    return -1;
}

// 単語を辞書に登録またはカウントアップする関数
void registerWord(WordInfo *dict, int *dict_size, const char *word) {
    int idx = findWord(dict, *dict_size, word);
    if (idx != -1) {
        dict[idx].count++;
    } else {
        int i = 0;
        while (word[i]) {
            dict[*dict_size].word[i] = word[i];
            i++;
        }
        dict[*dict_size].word[i] = '\0';
        dict[*dict_size].count = 1;
        (*dict_size)++;
    }
}

// 辞書を頻度降順・同頻度はアルファベット昇順にソートする関数
void sortDict(WordInfo *dict, int dict_size) {
    for (int i = 0; i < dict_size - 1; i++) {
        for (int j = 0; j < dict_size - 1 - i; j++) {
            int swap = 0;
            if (dict[j].count < dict[j+1].count) {
                swap = 1;
            } else if (dict[j].count == dict[j+1].count) {
                const char *a = dict[j].word;
                const char *b = dict[j+1].word;

                while (*a && *b && *a == *b) {
                    a++;
                    b++;
                }

                if (*a > *b)
                    swap = 1;
            }
            if (swap) {
                WordInfo tmp = dict[j];
                dict[j] = dict[j+1];
                dict[j+1] = tmp;
            }
        }
    }
}
