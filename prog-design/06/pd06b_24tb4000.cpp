// 課題： pd06b スタッククラス
// 氏名：　　　学生証番号：　　　日付：2026/  /
// 開発環境：Wandbox(online) https://wandbox.org/
// コンパイラー：gcc 13.2.0
// コマンド：g++ prog.cc -Wall -Wextra -std=c++17 -pedantic
// 説明：


#include <iostream>

#define N 5
#define ERROR -9999

class Stack {
// ここを作る
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
