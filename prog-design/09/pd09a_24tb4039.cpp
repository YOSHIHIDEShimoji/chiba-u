// 課題： pd09a 複素数クラスの演算子
// 氏名：下地慶英　学生証番号：24TB4039　日付：2026/06/30
// 開発環境：Wandbox(online) https://wandbox.org/
// コンパイラー：gcc 13.2.0
// コマンド：g++ prog.cc -Wall -Wextra -std=c++17 -pedantic
// 説明：
//   - pd08aのComplexクラスに四則演算の演算子をオーバーロードした
//   - 複素数同士、複素数と実数の +,-,*,/ を定義し結果を新しいComplexで返す
//   - ~で共役複素数、!で絶対値(double)を返す演算子を定義した
// 調べた関数：
//   - sqrt：平方根（絶対値の計算に使用）

#include <cstdio>
#include <cmath>

// 抽象クラス NumericData
class NumericData {
public:
    virtual void Show(int decimalPart = 3) = 0;
};

// Point2D クラス: x,y座標を実数型で持つ
class Point2D : public NumericData {
    double _x;
    double _y;

public:
    Point2D(double x, double y) {
        _x = x;
        _y = y;
    }
    void Show(int decimalPart = 3) {
        if (decimalPart == 2) printf("(x,y)=(%.2f, %.2f)\n", _x, _y);
        else                  printf("(x,y)=(%.3f, %.3f)\n", _x, _y);
    }
};

// Complex クラス: 実数rと虚数vの複素数
class Complex : public NumericData {
    double _r;
    double _v;

public:
    Complex(double r, double v) {
        _r = r;
        _v = v;
    }

    // 複素数同士の四則演算（結果を新しいComplexで返す）
    Complex operator+(const Complex &c) {
        return Complex(_r + c._r, _v + c._v);
    }
    Complex operator-(const Complex &c) {
        return Complex(_r - c._r, _v - c._v);
    }
    Complex operator*(const Complex &c) {
        return Complex(_r * c._r - _v * c._v, _r * c._v + _v * c._r);
    }
    Complex operator/(const Complex &c) {
        double d = c._r * c._r + c._v * c._v;
        return Complex((_r * c._r + _v * c._v) / d, (_v * c._r - _r * c._v) / d);
    }

    // 複素数と実数の四則演算
    Complex operator+(double r) {
        return Complex(_r + r, _v);
    }
    Complex operator-(double r) {
        return Complex(_r - r, _v);
    }
    Complex operator*(double r) {
        return Complex(_r * r, _v * r);
    }
    Complex operator/(double r) {
        return Complex(_r / r, _v / r);
    }

    // ~演算子：共役複素数を返す
    Complex operator~() {
        return Complex(_r, -_v);
    }

    // !演算子：絶対値を返す
    double operator!() {
        return sqrt(_r * _r + _v * _v);
    }

    void Show(int decimalPart = 3) {
        if (_v == 0.0) {
            // 虚部なし
            if (decimalPart == 2)       printf("%.2f\n", _r);
            else                        printf("%.3f\n", _r);
        } else if (_r == 0.0) {
            // 実部なし
            if (_v == 1.0)              printf("i\n");
            else if (_v == -1.0)        printf("-i\n");
            else if (decimalPart == 2)  printf("%.2fi\n", _v);
            else                        printf("%.3fi\n", _v);
        } else {
            // 実部と虚部あり
            if (decimalPart == 2)       printf("%.2f", _r);
            else                        printf("%.3f", _r);
            if (_v == 1.0)              printf("+i\n");
            else if (_v == -1.0)        printf("-i\n");
            else if (_v > 0.0) {
                if (decimalPart == 2)   printf("+%.2fi\n", _v);
                else                    printf("+%.3fi\n", _v);
            } else {
                if (decimalPart == 2)   printf("%.2fi\n", _v);
                else                    printf("%.3fi\n", _v);
            }
        }
    }
};

// ------------------------------------------------------------
// メイン関数：オーバーロードした演算子の動作検証
int main() {
    Complex a(3.0, 4.0);
    Complex b(1.0, 2.0);
    printf("a=3+4i, b=1+2i\n");

    // 複素数同士の四則演算
    Complex c_add = a + b;   // 4+6i
    Complex c_sub = a - b;   // 2+2i
    Complex c_mul = a * b;   // -5+10i
    Complex c_div = a / b;   // 2.2-0.4i
    printf("a+b="); c_add.Show(2);
    printf("a-b="); c_sub.Show(2);
    printf("a*b="); c_mul.Show(2);
    printf("a/b="); c_div.Show(2);

    // 複素数と実数の四則演算
    Complex r_add = a + 2.0;  // 5+4i
    Complex r_sub = a - 2.0;  // 1+4i
    Complex r_mul = a * 2.0;  // 6+8i
    Complex r_div = a / 2.0;  // 1.5+2i
    printf("a+2="); r_add.Show(2);
    printf("a-2="); r_sub.Show(2);
    printf("a*2="); r_mul.Show(2);
    printf("a/2="); r_div.Show(2);

    // 共役と絶対値
    Complex conj = ~a;        // 3-4i
    printf("~a="); conj.Show(2);
    printf("!a=%.2f\n", !a);  // 5.00

    return 0;
}
