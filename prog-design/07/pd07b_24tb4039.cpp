// 課題： pd07b Matrixクラス
// 氏名：下地慶英　学生証番号：24TB4039　日付：2026/06/15
// 開発環境：Wandbox(online) https://wandbox.org/
// コンパイラー：gcc 13.2.0
// コマンド：g++ prog.cc -Wall -Wextra -std=c++17 -pedantic
// 説明：Matrixクラスを実装した。

// 工夫した点：
//    - 1次元配列で2次元行列を管理し、data[r * _cols + c]でアクセスした
//    - コピーコンストラクタでディープコピーを実装し、元の行列に影響が出ないようにした
//    - Add(Matrix&)でサイズ不一致のエラーチェックを行った

#include <iostream>
#include <cstdio>

class Matrix {
    int _rows;
    int _cols;
    double *_data;

public:
    // コンストラクタ：行列のサイズを指定して零行列を生成
    // @param R：行数、C：列数
    Matrix(int R, int C) {
        _rows = R;
        _cols = C;
        _data = new double[_rows * _cols];
        for (int i = 0; i < _rows * _cols; i++) {
            _data[i] = 0.0;
        }
    }

    // コンストラクタ：行列のサイズと要素を指定して行列を生成
    // @param R：行数、C：列数、vals：要素の1次元配列
    Matrix(int R, int C, double *vals) {
        _rows = R;
        _cols = C;
        _data = new double[_rows * _cols];
        for (int i = 0; i < _rows * _cols; i++) {
            _data[i] = vals[i];
        }
    }

    // コンストラクタ：引数の行列と同じ値を持つ新しい行列を生成
    // @param m：複製元の行列
    Matrix(Matrix &m) {
        _rows = m._rows;
        _cols = m._cols;
        _data = new double[_rows * _cols];
        for (int i = 0; i < _rows * _cols; i++) {
            _data[i] = m._data[i];
        }
    }

    // デストラクタ：動的確保したメモリを解放する
    ~Matrix() {
        delete[] _data;
    }

    // 指定した1要素に値を代入する
    // @param r：行、c：列、v：値
    void Set(int r, int c, double v) {
        _data[r * _cols + c] = v;
    }

    // 全要素に値を代入する
    // @param vals：要素の1次元配列
    void Set(double *vals) {
        for (int i = 0; i < _rows * _cols; i++) {
            _data[i] = vals[i];
        }
    }

    // 指定した1要素の値を取得する
    // @param r：行、c：列
    double Get(int r, int c) {
        return _data[r * _cols + c];
    }

    // 全要素にスカラー値を加算する
    // @param v：加算する値
    void Add(double v) {
        for (int i = 0; i < _rows * _cols; i++) {
            _data[i] += v;
        }
    }

    // 引数の行列を加算する（サイズが一致する場合のみ）
    // @param m：加算する行列
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

// メイン関数
// 行列クラスの動作検証を行う
int main()
{
	// 零行列コンストラクタと表示テスト
    Matrix mat1(2, 2);
    std::cout << "mat1=\n";
    mat1.Show();

	// 単一要素代入テスト
    mat1.Set(0, 0, 1.0);
    mat1.Set(0, 1, 2.0);
    mat1.Set(1, 0, 3.0);
    mat1.Set(1, 1, 4.0);
    std::cout << "mat1=\n";
    mat1.Show();

    // 単一要素取得テスト
    std::cout << "mat1(0,1)=" << mat1.Get(0,1) << "\n\n";

	// 要素指定コンストラクタのテスト
	double vals[4] = {5.1, 6.2, 7.3, 8.4};
    Matrix mat2(2, 2, vals);
    std::cout << "mat2=\n";
    mat2.Show();

    // 複製コンストラクタのテスト
    Matrix mat3(mat1);
    std::cout << "mat3=\n";
    mat3.Show();

    // 行列にスカラー値加算テスト
    mat3.Add(0.5);
    std::cout << "mat3=\n";
    mat3.Show();
    std::cout << "mat1=\n";
    mat1.Show();	// mat3 複製元のmat1は変化無いことを確認

	// 行列加算テスト
	mat3.Add(mat2);
    std::cout << "mat3=\n";
    mat3.Show();
    std::cout << "mat2=\n";
    mat2.Show();	// mat2は変化無いことを確認

    // サイズの異なる行列同士を加算するテスト
    Matrix mat4(3, 3);
    mat3.Add(mat4);    // 2x2 + 3x3 はエラーになるはず

    // 全要素一括代入するテスト
    double vals2[4] = {9.9, 8.8, 7.7, 6.6};
    mat1.Set(vals2);
    std::cout << "mat1=\n";
    mat1.Show();

    return 0;
}
