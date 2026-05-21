/* 課題： pd05b 行列演算の一般化
 * 氏名：　　　学生証番号：　　　日付：2026/  /
 * 説明：
 * 開発環境：Wandbox(online) https://wandbox.org/
 * コンパイラー：gcc 13.2.0
 * コマンド：gcc -Wall -Wextra -std=c11 -pedantic
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN 256
#define MATRIX_FILE "matrix.csv"

/* 行列構造体 */
typedef struct {
    int rows;   /* 行数 */
    int cols;   /* 列数 */
    double **val;   /* 要素の値 */
} Matrix;

/* 行列のメモリ確保 */
/* 引数 m : 行列（更新） */
void CreateMatrix(Matrix *m) {
    m->val = (double**)malloc(sizeof(double*) * 2);
    for (int r = 0; r < 2; r++){
        m->val[r] = (double*)malloc(sizeof(double) * 2);
    }
}

/* 行列のメモリ解放 */
/* 引数 m : 行列（更新） */
void ReleaseMatrix(Matrix *m) {
    for (int r = 0; r < 2; r++){
        free(m->val[r]);
    }
    free(m->val);
}

/* ファイルから行列を読み込み */
/* 引数 filename : ファイル名（入力） */
/* 引数 m : 基の行列（出力） */
void LoadMatrix(char *filename, Matrix *m) {
    FILE *fp = fopen(filename, "r");
    for (int r = 0; r < 2; r++){
        char buf[LINE_LEN];
        fgets(buf, LINE_LEN, fp);
        if (strlen(buf) == LINE_LEN - 1) {
            fprintf(stderr, "行列ファイルの1行文字数は最大%dです．", LINE_LEN);
            exit(1);
        }
        char *p = strtok(buf, ",");
        int c = 0;
        m->val[r][c ++] = atof(p);
        while ((p = strtok(NULL, ",")) != NULL){
            m->val[r][c ++] = atof(p);
        }
    }
    fclose(fp);
}

/* 行列表示 */
/* 引数 m : 行列（入力） */
void ShowMatrix(Matrix *m) {
    for (int r = 0; r < 2; r++){
        printf("[");
        for (int c = 0; c < 2; c++){
            printf("%7.2f", m->val[r][c]);
            if (c < 1) {
                printf(",");
            }
        }
        printf("]\n");
    }
}

/* 逆行列の計算 */
/* 引数 m : 基の行列（入力） */
/* 引数 inv : 逆行列（出力） */
/* 戻り値(double) : 行列式 */
double InverseMatrix(Matrix *m, Matrix *inv) {
    double det = m->val[0][0] * m->val[1][1] - m->val[0][1] * m->val[1][0];
    inv->val[0][0] = m->val[1][1] / det;
    inv->val[0][1] = - m->val[0][1] / det;
    inv->val[1][0] = - m->val[1][0] / det;
    inv->val[1][1] = m->val[0][0] / det;
    return det;
}

/* メイン関数 */
int main()
{
    Matrix m;
    CreateMatrix(&m);
    LoadMatrix(MATRIX_FILE, &m);
    printf("Original matrix =\n");
    ShowMatrix(&m);

    Matrix inv;
    CreateMatrix(&inv);
    double det = InverseMatrix(&m, &inv);
    printf("Determinant = %.2f\n", det);
    printf("Inverse matrix =\n");
    ShowMatrix(&inv);
    
    ReleaseMatrix(&m);
    ReleaseMatrix(&inv);

    return 0;
}
