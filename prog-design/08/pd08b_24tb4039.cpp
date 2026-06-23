// 課題： pd08b Imageクラス
// 氏名：下地慶英　学生証番号：24TB4039　日付：2026/06/21
// 開発環境：Wandbox(online) https://wandbox.org/
// コンパイラー：gcc 13.2.0
// コマンド：g++ prog.cc -Wall -Wextra -std=c++17 -pedantic
// 説明：
//   - pd07bのMatrixクラスのメンバ変数をprotectedに変更してImageクラスを継承
//   - DrawLineはBresenhamのアルゴリズムで線分を描画
//   - Rotateは90度ずつ右回転を繰り返して実現

#include <iostream>
#include <cstdio>
#include <cstdlib>

// Matrix クラス: pd07bのコピー（アクス権限を protected に修正）
class Matrix {
protected:
    int _rows;
    int _cols;
    double *_data;

public:
    // コンストラクタ：行列のサイズを指定して零行列を生成
    Matrix(int R, int C) {
        _rows = R;
        _cols = C;
        _data = new double[_rows * _cols];
        for (int i = 0; i < _rows * _cols; i++) {
            _data[i] = 0.0;
        }
    }

    // コンストラクタ：行列のサイズと要素を指定して行列を生成
    Matrix(int R, int C, double *vals) {
        _rows = R;
        _cols = C;
        _data = new double[_rows * _cols];
        for (int i = 0; i < _rows * _cols; i++) {
            _data[i] = vals[i];
        }
    }

    // コンストラクタ：引数の行列と同じ値を持つ新しい行列を生成
    Matrix(Matrix &m) {
        _rows = m._rows;
        _cols = m._cols;
        _data = new double[_rows * _cols];
        for (int i = 0; i < _rows * _cols; i++) {
            _data[i] = m._data[i];
        }
    }

    ~Matrix() {
        delete[] _data;
    }

    // 指定した1要素に値を代入する
    void Set(int r, int c, double v) {
        _data[r * _cols + c] = v;
    }

    // 全要素に値を代入する
    void Set(double *vals) {
        for (int i = 0; i < _rows * _cols; i++) {
            _data[i] = vals[i];
        }
    }

    // 指定した1要素の値を取得する
    double Get(int r, int c) {
        return _data[r * _cols + c];
    }

    // 全要素にスカラー値を加算する
    void Add(double v) {
        for (int i = 0; i < _rows * _cols; i++) {
            _data[i] += v;
        }
    }

    // 引数の行列を加算する（サイズが一致する場合のみ）
    void Add(Matrix &m) {
        if (_rows != m._rows || _cols != m._cols) {
            std::cout << "Add error: サイズが異なります\n\n";
            return;
        }
        for (int i = 0; i < _rows * _cols; i++) {
            _data[i] += m._data[i];
        }
    }

    // 格納されている行列を画面に表示する
    // 整数部3桁・小数部2桁、両端を|で囲む
    void Show() {
        for (int r = 0; r < _rows; r++) {
            std::cout << "|";
            for (int c = 0; c < _cols; c++) {
                printf("%6.2f", _data[r * _cols + c]);
            }
            std::cout << "|\n";
        }
        std::cout << "\n";
    }
};

// Image クラス: Matrixを継承し画像処理機能を追加
class Image : public Matrix {
public:
    Image(int R, int C) : Matrix(R, C) {}

    // (x1,y1)から(x2,y2)へ値vの線分を描画する
    // x=列, y=行 に対応
    void DrawLine(int x1, int y1, int x2, int y2, int v) {
        int dx = x2 - x1;
        int dy = y2 - y1;
        int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);
        double xStep = (double)dx / steps;
        double yStep = (double)dy / steps;
        double x = x1;
        double y = y1;
        for (int i = 0; i <= steps; i++) {
            Set((int)(y + 0.5), (int)(x + 0.5), (double)v);
            x += xStep;
            y += yStep;
        }
    }

    // 画像を右回りに90度単位で回転する
    // @param angle=0:回転なし, 1:90度, 2:180度, 3:270度
    void Rotate(int angle) {
        int times = angle % 4;
        for (int t = 0; t < times; t++) {
            // 90度右回転
            double *tmp = new double[_cols * _rows];
            int newRows = _cols;
            int newCols = _rows;
            for (int r = 0; r < newRows; r++) {
                for (int c = 0; c < newCols; c++) {
                    tmp[r * newCols + c] = _data[(_rows - 1 - c) * _cols + r];
                }
            }
            delete[] _data;
            _data = tmp;
            _rows = newRows;
            _cols = newCols;
        }
    }

    // 画像を表示する
    void ShowImage() {
        for (int r = 0; r < _rows; r++) {
            for (int c = 0; c < _cols; c++) {
                printf("%d", (int)_data[r * _cols + c] % 10);
            }
            printf("\n");
        }
        printf("\n");
    }
};

// メイン関数: Imageクラスの動作検証
int main() {
    // スライドの例
    Image img(4, 4);
    img.Set(1, 2, 5);
    img.ShowImage();
    img.DrawLine(0, 0, 1, 3, 2);
    img.ShowImage();
    img.Rotate(1);
    img.ShowImage();

    // 180度・270度回転
    Image img2(4, 4);
    img2.DrawLine(0, 0, 3, 0, 1);  // 水平線
    img2.DrawLine(0, 0, 0, 3, 2);  // 垂直線
    std::cout << "img2 original:\n";
    img2.ShowImage();
    img2.Rotate(2);
    std::cout << "img2 Rotate(2):\n";
    img2.ShowImage();

    // DrawLineの斜め線
    Image img3(5, 5);
    img3.DrawLine(0, 0, 4, 4, 9);
    std::cout << "img3 diagonal:\n";
    img3.ShowImage();

    return 0;
}
