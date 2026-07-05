// 課題： pd10b 行列クラスの汎用化
// 氏名：下地慶英　学生証番号：24TB4039　日付：2026/07/05
// 開発環境：Wandbox(online) https://wandbox.org/
// コンパイラー：gcc 13.2.0
// コマンド：g++ prog.cc -Wall -Wextra -std=c++17 -pedantic
// 説明：
//   - pd09bのMatrixクラスをテンプレート化し、任意のデータ型(MatrixType)で使えるようにした
//   - Show()は要素をdoubleにキャストしてから出力することで、どの型でも整形して表示できるようにした
//   - 動作確認としてdouble型に加えint型でも同じMatrixクラスを使ってみた
// 発展：
//   - 要素の型サイズ(sizeof)を表示するShowSize()を追加した
// 調べた関数：
//   - fabs：絶対値

#include <iostream>
#include <cstdio>
#include <cmath>

template <class MatrixType>
class Matrix {
    int _rows;
    int _cols;
    MatrixType *_data;

public:
    // コンストラクタ：サイズを指定して零行列を生成
    Matrix(int R, int C) {
        _rows = R;
        _cols = C;
        _data = new MatrixType[_rows * _cols];
        for (int i = 0; i < _rows * _cols; i++) {
            _data[i] = 0;
        }
    }

    // コンストラクタ：サイズと要素を指定して生成
    Matrix(int R, int C, MatrixType *vals) {
        _rows = R;
        _cols = C;
        _data = new MatrixType[_rows * _cols];
        for (int i = 0; i < _rows * _cols; i++) {
            _data[i] = vals[i];
        }
    }

    // コピーコンストラクタ：動的配列の要素をコピーして複製
    Matrix(const Matrix &m) {
        _rows = m._rows;
        _cols = m._cols;
        _data = new MatrixType[_rows * _cols];
        for (int i = 0; i < _rows * _cols; i++) {
            _data[i] = m._data[i];
        }
    }

    // デストラクタ：動的確保したメモリを解放
    ~Matrix() {
        delete[] _data;
    }

    // 代入演算子：既存の配列を解放してから複製する
    Matrix& operator=(const Matrix &m) {
        delete[] _data;
        _rows = m._rows;
        _cols = m._cols;
        _data = new MatrixType[_rows * _cols];
        for (int i = 0; i < _rows * _cols; i++) {
            _data[i] = m._data[i];
        }
        return *this;
    }

    // 指定要素に値を代入
    void Set(int r, int c, MatrixType v) {
        _data[r * _cols + c] = v;
    }

    // 指定要素の値を取得
    MatrixType Get(int r, int c) {
        return _data[r * _cols + c];
    }

    // 行列同士の加算（結果を新しい行列で返す。不一致時は零行列を返す）
    Matrix operator+(const Matrix &m) {
        if (_rows != m._rows || _cols != m._cols) {
            std::cout << "operator+ error: サイズが異なります\n";
            return Matrix(_rows, _cols);
        }
        Matrix ret(_rows, _cols);
        for (int i = 0; i < _rows * _cols; i++) {
            ret._data[i] = _data[i] + m._data[i];
        }
        return ret;
    }

    // 行列同士の減算
    Matrix operator-(const Matrix &m) {
        if (_rows != m._rows || _cols != m._cols) {
            std::cout << "operator- error: サイズが異なります\n";
            return Matrix(_rows, _cols);
        }
        Matrix ret(_rows, _cols);
        for (int i = 0; i < _rows * _cols; i++) {
            ret._data[i] = _data[i] - m._data[i];
        }
        return ret;
    }

    // 行列同士の積（this:_rows×_cols, m:_cols×m._cols）
    Matrix operator*(const Matrix &m) {
        if (_cols != m._rows) {
            std::cout << "operator* error: サイズが不適合です\n";
            return Matrix(_rows, m._cols);
        }
        Matrix ret(_rows, m._cols);
        for (int i = 0; i < _rows; i++) {
            for (int j = 0; j < m._cols; j++) {
                MatrixType s = 0;
                for (int k = 0; k < _cols; k++) {
                    s += _data[i * _cols + k] * m._data[k * m._cols + j];
                }
                ret._data[i * m._cols + j] = s;
            }
        }
        return ret;
    }

    // スカラ値との四則演算
    Matrix operator+(MatrixType v) {
        Matrix ret(_rows, _cols);
        for (int i = 0; i < _rows * _cols; i++) ret._data[i] = _data[i] + v;
        return ret;
    }
    Matrix operator-(MatrixType v) {
        Matrix ret(_rows, _cols);
        for (int i = 0; i < _rows * _cols; i++) ret._data[i] = _data[i] - v;
        return ret;
    }
    Matrix operator*(MatrixType v) {
        Matrix ret(_rows, _cols);
        for (int i = 0; i < _rows * _cols; i++) ret._data[i] = _data[i] * v;
        return ret;
    }
    Matrix operator/(MatrixType v) {
        Matrix ret(_rows, _cols);
        for (int i = 0; i < _rows * _cols; i++) ret._data[i] = _data[i] / v;
        return ret;
    }

    // 複合代入演算子
    Matrix& operator+=(const Matrix &m) { *this = *this + m; return *this; }
    Matrix& operator-=(const Matrix &m) { *this = *this - m; return *this; }
    Matrix& operator*=(const Matrix &m) { *this = *this * m; return *this; }

    // ~演算子：転置行列を返す
    Matrix operator~() {
        Matrix t(_cols, _rows);
        for (int r = 0; r < _rows; r++) {
            for (int c = 0; c < _cols; c++) {
                t._data[c * _rows + r] = _data[r * _cols + c];
            }
        }
        return t;
    }

    // !演算子：逆行列を返す（ガウス・ジョルダン法、正方行列が前提）
    Matrix operator!() {
        int n = _rows;
        Matrix a(*this);    // 計算用に自分を複製
        Matrix inv(n, n);   // 単位行列から開始
        for (int i = 0; i < n; i++) inv._data[i * n + i] = 1;

        for (int i = 0; i < n; i++) {
            // ピボット選択：i列で絶対値最大の行を選び入れ替える
            int pivot = i;
            for (int k = i + 1; k < n; k++) {
                if (fabs((double)a._data[k * n + i]) > fabs((double)a._data[pivot * n + i])) {
                    pivot = k;
                }
            }
            if (pivot != i) {
                for (int j = 0; j < n; j++) {
                    MatrixType t = a._data[i * n + j];
                    a._data[i * n + j] = a._data[pivot * n + j];
                    a._data[pivot * n + j] = t;
                    t = inv._data[i * n + j];
                    inv._data[i * n + j] = inv._data[pivot * n + j];
                    inv._data[pivot * n + j] = t;
                }
            }
            // ピボットを1に正規化
            MatrixType p = a._data[i * n + i];
            for (int j = 0; j < n; j++) {
                a._data[i * n + j] /= p;
                inv._data[i * n + j] /= p;
            }
            // 他の行のi列を0にする
            for (int k = 0; k < n; k++) {
                if (k == i) continue;
                MatrixType f = a._data[k * n + i];
                for (int j = 0; j < n; j++) {
                    a._data[k * n + j] -= f * a._data[i * n + j];
                    inv._data[k * n + j] -= f * inv._data[i * n + j];
                }
            }
        }
        return inv;
    }

    // 行列を表示する（整数部3桁・小数部2桁、両端を|で囲む）
    void Show() {
        for (int r = 0; r < _rows; r++) {
            std::cout << "|";
            for (int c = 0; c < _cols; c++) {
                printf("%6.2f", (double)_data[r * _cols + c]);
            }
            std::cout << "|\n";
        }
        std::cout << "\n";
    }

    // 発展：要素の型サイズを表示する
    void ShowSize() {
        std::cout << "要素の型サイズ: " << sizeof(MatrixType) << " bytes\n";
    }
};

// メイン関数：テンプレート化したMatrixクラスの動作検証
int main() {
    double av[4] = {1, 2, 3, 4};
    double bv[4] = {5, 6, 7, 8};
    Matrix<double> A(2, 2, av);
    Matrix<double> B(2, 2, bv);
    std::cout << "A=\n"; A.Show();
    std::cout << "B=\n"; B.Show();

    Matrix<double> C = A + B;                // コピーコンストラクタ
    std::cout << "A+B=\n"; C.Show();
    C = A - B;                               // 代入演算子
    std::cout << "A-B=\n"; C.Show();
    C = A * B;
    std::cout << "A*B=\n"; C.Show();

    C = A + 10.0;  std::cout << "A+10=\n"; C.Show();
    C = A - 1.0;   std::cout << "A-1=\n";  C.Show();
    C = A * 2.0;   std::cout << "A*2=\n";  C.Show();
    C = A / 2.0;   std::cout << "A/2=\n";  C.Show();

    C = ~A;        std::cout << "~A (転置)=\n";       C.Show();
    C = !A;        std::cout << "!A (逆行列)=\n";     C.Show();
    C = A * (!A);  std::cout << "A*(!A) (単位行列)=\n"; C.Show();

    // 複合代入演算子
    Matrix<double> acc = A;
    acc += B;  std::cout << "A+=B=\n"; acc.Show();
    acc -= B;  std::cout << "(A+=B)-=B=\n"; acc.Show();   // Aに戻る

    // サイズ不一致の検出
    std::cout << "サイズ不一致の加算テスト:\n";
    Matrix<double> D(3, 3);
    Matrix<double> err = A + D;   // 2x2 + 3x3 はエラーメッセージが出る
    err.Show();                  // 返ってきた零行列

    // --- 連立方程式を解く ---
    //  -3w + 8x + 3y - 3z = -43
    //   4w -  x + 5y + 3z = -12
    //    w - 2x +  y - 2z = -12
    //  -3w + 5x + 8y -  z = -52
    double coef[16] = {
        -3,  8,  3, -3,
         4, -1,  5,  3,
         1, -2,  1, -2,
        -3,  5,  8, -1
    };
    double rhs[4] = {-43, -12, -12, -52};
    Matrix<double> Amat(4, 4, coef);
    Matrix<double> Bvec(4, 1, rhs);

    Matrix<double> X = (!Amat) * Bvec;   // X = A^-1 * B
    std::cout << "連立方程式の解 (w, x, y, z)=\n";
    X.Show();

    // 検算：求めた解を元の行列に掛けて右辺Bに戻るか確認
    Matrix<double> check = Amat * X;
    std::cout << "検算 A*X (右辺 -43,-12,-12,-52 に一致)=\n";
    check.Show();

    // --- int型での動作確認 ---
    int iv1[4] = {1, 2, 3, 4};
    int iv2[4] = {5, 6, 7, 8};
    Matrix<int> IA(2, 2, iv1);
    Matrix<int> IB(2, 2, iv2);
    std::cout << "int型 IA=\n"; IA.Show();
    std::cout << "int型 IB=\n"; IB.Show();

    Matrix<int> IC = IA + IB;
    std::cout << "IA+IB=\n"; IC.Show();
    IC = IA * 2;
    std::cout << "IA*2=\n"; IC.Show();
    IC = ~IA;
    std::cout << "~IA (転置)=\n"; IC.Show();

    A.ShowSize();
    IA.ShowSize();

    return 0;
}
