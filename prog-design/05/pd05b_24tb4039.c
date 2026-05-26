/* 課題： pd05b 行列演算の一般化
 * 氏名：下地慶英   学生証番号：24TB4039    日付：2026/05/26
 * 説明：・matrices.csvにおいて、'#'からはじまる行を無視し、行列ファイルにコメント行を許容できるようにした。
 *      ・任意サイズに対応するため行列データはmallocで動的に確保した。
 * 開発環境：Wandbox(online) https://wandbox.org/
 * コンパイラー：gcc 13.2.0
 * コマンド：gcc -Wall -Wextra -std=c11 -pedantic
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN 256
#define MATRICES_FILE "matrices.csv"

/* 行列構造体 */
typedef struct {
    int rows;   /* 行数 */
    int cols;   /* 列数 */
    double **val;   /* 要素の値 */
} Matrix;

/* 行列のメモリ確保 */
/* 引数 m : 行列（更新） */
void CreateMatrix(Matrix *m) {
    m->val = (double **)malloc(sizeof(double *) * m->rows);
    for (int r = 0; r < m->rows; r++) {
        m->val[r] = (double *)malloc(sizeof(double) * m->cols);
    }
}

/* 行列のメモリ解放 */
/* 引数 m : 行列（更新） */
void ReleaseMatrix(Matrix *m) {
    for (int r = 0; r < m->rows; r++) {
        free(m->val[r]);
    }
    free(m->val);
}

/* ファイルから次の行列を読み込む */
/* 引数 fp : ファイルポインタ（入力） */
/* 引数 m : 行列（出力） */
/* 戻り値(int) : 読み込めたら1，EOFなら0 */
int LoadNextMatrix(FILE *fp, Matrix *m) {
    char buf[LINE_LEN];

    while (fgets(buf, LINE_LEN, fp)) {
        if (buf[0] != '\n' && buf[0] != '\r' && buf[0] != '#') break;
    }

    int rows, cols;
    if (sscanf(buf, "%d,%d", &rows, &cols) != 2) return 0;

    m->rows = rows;
    m->cols = cols;
    CreateMatrix(m);

    for (int r = 0; r < rows; r++) {
        fgets(buf, LINE_LEN, fp);
        if (strlen(buf) == LINE_LEN - 1) {
            printf("行列ファイルの1行文字数は最大%dです．", LINE_LEN);
            exit(1);
        }
        char *p = strtok(buf, ",");
        int c = 0;
        m->val[r][c ++] = atof(p);
        while ((p = strtok(NULL, ",")) != NULL) {
            m->val[r][c ++] = atof(p);
        }
    }

    return 1;
}

/* 行列表示 */
/* 引数 m : 行列（入力） */
void ShowMatrix(Matrix *m) {
    for (int r = 0; r < m->rows; r++) {
        printf("[");
        for (int c = 0; c < m->cols; c++) {
            printf("%7.2f", m->val[r][c]);
            if (c < m->cols - 1) printf(",");
        }
        printf("]\n");
    }
    printf("\n");
}

/* 逆行列の計算 */
/* 引数 m : 基の行列（入力） */
/* 引数 inv : 逆行列（出力） */
/* 戻り値(double) : 行列式（特異行列は0.0） */
double InverseMatrix(Matrix *m, Matrix *inv) {
    int n = m->rows;
    int w = 2 * n;

    double *aug = (double *)malloc(sizeof(double) * n * w);

    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) aug[r * w + c] = m->val[r][c];
        for (int c = 0; c < n; c++) aug[r * w + n + c] = (r == c) ? 1.0 : 0.0;
    }

    double det = 1.0;

    for (int col = 0; col < n; col++) {
        if (aug[col * w + col] == 0.0) { free(aug); return 0.0; }

        det *= aug[col * w + col];
        double pivot = aug[col * w + col];
        for (int c = 0; c < w; c++) aug[col * w + c] /= pivot;

        for (int r = 0; r < n; r++) {
            if (r == col) continue;
            double factor = aug[r * w + col];
            for (int c = 0; c < w; c++) aug[r * w + c] -= factor * aug[col * w + c];
        }
    }

    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) inv->val[r][c] = aug[r * w + n + c];
    }

    free(aug);
    return det;
}

/* メイン関数 */
int main(void) {
    FILE *fp = fopen(MATRICES_FILE, "r");
    if (!fp) {
        printf("Can't open the file: %s\n", MATRICES_FILE);
        return 1;
    }

    int matrixNum = 0;
    Matrix m;

    while (LoadNextMatrix(fp, &m)) {
        matrixNum++;
        printf("=== Matrix %d ===\n", matrixNum);

        if (m.rows != m.cols) {
            printf("error: Non-square matrix (%dx%d)\n\n", m.rows, m.cols);
            ReleaseMatrix(&m);
            continue;
        }

        printf("Original matrix =\n");
        ShowMatrix(&m);

        Matrix inv;
        inv.rows = m.rows;
        inv.cols = m.cols;
        CreateMatrix(&inv);

        double det = InverseMatrix(&m, &inv);
        printf("Determinant = %.2f\n", det);

        if (det == 0.0) {
            printf("error: There is no inverse matrix（det = 0）\n\n");
            ReleaseMatrix(&m);
            ReleaseMatrix(&inv);
            continue;
        }

        printf("Inverse matrix =\n");
        ShowMatrix(&inv);

        ReleaseMatrix(&m);
        ReleaseMatrix(&inv);
    }

    fclose(fp);
    return 0;
}
