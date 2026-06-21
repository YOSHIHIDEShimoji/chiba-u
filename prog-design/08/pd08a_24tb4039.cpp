// 課題： pd08a インターフェイス
// 氏名：下地慶英　学生証番号：24TB4039　日付：2026/06/21
// 開発環境：Wandbox(online) https://wandbox.org/
// コンパイラー：gcc 13.2.0
// コマンド：g++ prog.cc -Wall -Wextra -std=c++17 -pedantic
// 説明：
//   - NumericDataを純粋仮想関数Show()を持つ抽象クラスとして実装
//   - Point2DとComplexがNumericDataを継承し、ポリモーフィズムで表示
// 調べた関数：printf("%.*f", n, x) - 小数桁数を実行時に指定できる書式

#include <cstdio>

// 抽象クラス NumericData
class NumericData {
public:
    virtual void Show(int decimalPart = 3) = 0;
    virtual ~NumericData() {}
};

// Point2D クラス: x,y座標を実数型で持つ
class Point2D : public NumericData {
    double _x;
    double _y;
public:
    Point2D(double x, double y) : _x(x), _y(y) {}
    void Show(int decimalPart = 3) {
        printf("(x,y)=(%.*f, %.*f)\n", decimalPart, _x, decimalPart, _y);
    }
};

// Complex クラス: 実数rと虚数vの複素数
class Complex : public NumericData {
    double _r;
    double _v;
public:
    Complex(double r, double v) : _r(r), _v(v) {}
    void Show(int decimalPart = 3) {
        if (_v == 0.0) {
            // 虚部なし
            printf("%.*f\n", decimalPart, _r);
        } else if (_r == 0.0) {
            // 実部なし
            if (_v == 1.0)       printf("i\n");
            else if (_v == -1.0) printf("-i\n");
            else                 printf("%.*fi\n", decimalPart, _v);
        } else {
            // 実部と虚部あり
            printf("%.*f", decimalPart, _r);
            if (_v == 1.0)       printf("+i\n");
            else if (_v == -1.0) printf("-i\n");
            else if (_v > 0.0)   printf("+%.*fi\n", decimalPart, _v);
            else                 printf("%.*fi\n", decimalPart, _v);
        }
    }
};

// ------------------------------------------------------------
// メイン関数
//
// 期待される出力
// (x,y)=(4.00, -6.50)
// (x,y)=(-1.50, 2.00)
// 1.00+2.00i
// -2.00+i
// 1.00-4.40i
// 1.50i
// 3.14
int main() {
    Point2D p1(4.0, -6.5);
    Point2D p2(-1.5, 2.0);
    Complex c1(1.0, 2.0);
    Complex c2(-2.0, 1.0);
    Complex c3(1.0, -4.4);
    Complex c4(0.0, 1.5);
    Complex c5(3.141, 0);

    NumericData* values[] = {&p1, &p2, &c1, &c2, &c3, &c4, &c5};

    for (NumericData* pData : values) {
        pData->Show(2);
    }

    return 0;
}
