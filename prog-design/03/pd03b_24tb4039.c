/* 課題名 pd03b ソースコード整形
 * 氏名：下地慶英   学生証番号：24TB4039    日付：2026/05/12

 * 説明：

 * 開発環境：Wandbox(online) https://wandbox.org/
 * コンパイラー：gcc 13.2.0
 * コマンド：gcc -Wall -Wextra -std=c11 -pedantic
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_indent(int depth);

#define MAX_LINE 1000
#define INDENT 4

/*
 *@param String "fileName.c"
 */
int main(int argc, char **argv)
{
    // 引数がなければエラー
    char filename[1000];
    if (argc >= 2) {
        strcpy(filename, argv[1]);
    } else {
        printf("Usage: %s fileName.c\n", argv[0]);
        exit(1);
    }

    // ファイルを開く
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Can't open the file.\n");
        exit(1);
    }

    char line[MAX_LINE];
    int in_comment = 0;
    int paren_depth = 0;
    int depth = 0;
    int init_depth = 0;
    char last_char = 0;

    while (fgets(line, sizeof(line), fp)) {

        // 改行コードを消して、一つの文として扱う
        int len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
            line[--len] = '\0';

        // 空白 or \t が来たら、スキップする
        char *p = line;
        while (*p == ' ' || *p == '\t') 
            p++;
        if (*p == '\0')
            continue;
        
        // コメントの中はスキップ
        if (in_comment) {
            printf("%s\n", line);
            if (strstr(line, "*/"))
                in_comment = 0;
            continue;
        }
        
        // コメントの中はスキップ
        if (*p == '/' && *(p+1) == '*') {
            printf("%s\n", line);
            if (!strstr(line, "*/"))
                in_comment = 1;
            continue;
        }
        
        // 1行コメント(//) と プリプロセッサ(#) はスキップ
        if (*p == '#' || (*p == '/' && *(p+1) == '/')) {
            printf("%s\n", line);
            continue;
        }

        int in_string = 0;
        int need_indent = 1;

        // 1 文字ずつ *p に入れて処理する
        for (; *p != '\0'; p++) {
            char c = *p;
            
            // 文字列内 "*" を無視する
            if (c == '"') 
                in_string = !in_string;
            if (in_string) {
                if (need_indent) {
                    print_indent(depth);
                    need_indent = 0;
                }
                putchar(c);
                continue;
            }

            if (c == ' ' || c == '\t') {
                if (need_indent) continue;
                putchar(c);
                continue;
            }

            // '()', ')' の深さを保持
            if (c == '(') paren_depth++;
            if (c == ')') paren_depth--;

            // '{' の処理
            if (c == '{') {
                if (last_char == '=' || init_depth > 0) {
                    putchar('{');
                    init_depth++;
                } else {
                    if (!need_indent) 
                        putchar(' ');
                    printf("{\n");
                    depth++;
                    need_indent = 1;
                }
                last_char = '{';
                continue;
            }

            // '}' の処理
            if (c == '}') {
                if (init_depth > 0) {
                    putchar('}');
                    init_depth--;
                } else {
                    if (!need_indent) putchar('\n');
                    if (depth > 0) 
                        depth--;
                    print_indent(depth);
                    printf("}\n");
                    need_indent = 1;
                }
                last_char = '}';
                continue;
            }

            // ';' の処理
            if (c == ';') {
                putchar(';');
                if (paren_depth == 0) { 
                    putchar('\n');
                    need_indent = 1;
                }
                last_char = ';';
                continue;
            }

            // その他の文字
            if (need_indent) {
                print_indent(depth);
                need_indent = 0;
            }
            putchar(c);

            // 最後の文字を保持
            if (c != ' ' && c != '\t') {
                last_char = c;
            }

        }

        if (!need_indent) putchar('\n');
    }

    fclose(fp);
    return 0;
}

/*
 * indent の数だけ字下げする関数
 * @param int indent
 */
void print_indent(int depth) {
    for (int i = 0; i < depth * INDENT; i++) putchar(' ');
}
