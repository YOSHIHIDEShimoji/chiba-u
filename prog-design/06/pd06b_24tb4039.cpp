// 課題：pd06b スタッククラス
// 氏名：下地慶英　学生証番号：24tb4039　日付：2026/06/03
// 開発環境：Wandbox(online) https://wandbox.org/
// コンパイラー：gcc 13.2.0
// コマンド：g++ prog.cc -Wall -Wextra -std=c++17 -pedantic
// 説明：Stackクラスを実装した。
//       push()で追加、pop()で取り出し。失敗時はERROR(-9999)を返す。
//       デストラクタで残っている要素数を表示する。
//       発展：サイズ上限なしの仕様にした。容量が足りなくなったらnew[]で2倍に拡張する。
// 調べた関数：
//   - new[] / delete[] : C++の動的配列の確保と解放。Cのmalloc/freeに相当する。

#include <iostream>

#define N 5
#define ERROR -9999

class Stack {
    int *data;     // 動的配列
    int top;       // 現在の要素数
    int capacity;  // 確保済み容量

public:
    // コンストラクタ：最初はN個分確保する
    Stack() {
        capacity = N;
        data = new int[capacity];
        top = 0;
    }

    // デストラクタ：残っている要素数を表示してからメモリを解放する
    ~Stack() {
        if (top > 0) {
            std::cout << top << " values remain in stack\n";
        }
        delete[] data;
    }

    // スタックに数値を追加する
    int push(int value) {
        // 容量を超えたら配列を拡張する
        if (top >= capacity) {
            capacity *= 2;
            int *newData = new int[capacity];
            for (int i = 0; i < top; i++) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
        }
        data[top] = value;
        top++;
        return value;
    }

    // スタックから数値を取り出す
    int pop() {
        if (top <= 0) return ERROR;
        top--;
        return data[top];
    }
};

// 動作検証（テスト）
int main()
{
    Stack s;

    // スタックの基本機能確認
    std::cout << "1st push " << s.push(10) << "\n";
    std::cout << "2nd push " << s.push(20) << "\n";
    std::cout << "3rd push " << s.push(30) << "\n";
    std::cout << "1st pop  " << s.pop() << "\n";
    std::cout << "2nd pop  " << s.pop() << "\n";
    std::cout << "3rd pop  " << s.pop() << "\n";

    // エラーの検証 N+1回push,pop実施
    for (int i = 0; i < N + 1; i ++) {
        std::cout << i+1 << " push " << s.push(i) << "\n";
    }
    for (int i = 0; i < N + 1; i ++) {
        std::cout << i+1 << " pop " << s.pop() << "\n";
    }

    // 終了時処理の検証（2個pushして終了）
    s.push(10);
    s.push(20);

    return 0;
}
