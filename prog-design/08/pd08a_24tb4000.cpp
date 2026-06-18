// 課題： pd08a インターフェイス 課題
// 氏名：　　　学生証番号：　　　日付：2026/  /
// 開発環境：Wandbox(online) https://wandbox.org/
// コンパイラー：gcc 13.2.0
// コマンド：g++ prog.cc -Wall -Wextra -std=c++17 -pedantic

// 開発環境：Coddy(online) https://coddy.tech/playground/ja/cpp
// コンパイラー：gcc 14.2.0

// 説明：

#include <iostream>

// 抽象クラス NumericData
// 作る


// Point2D クラス
// 作る


// Complex クラス
// 作る


// ------------------------------------------------------------
// メイン関数
//
// 期待される出力
// (x,y)=(4.00, -6.50)
// (x,y)=(-1.50, 2.00)
// 1.00+2.00i
// -2.00+i
// 1.00-4.00i
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

    for (NumericData* pData :values) {
        pData->Show(2);
    }

    return 0;
}
