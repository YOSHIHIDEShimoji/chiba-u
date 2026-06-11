// 課題： pd07a 今日からの日数
// 氏名：　　　学生証番号：　　　日付：2026/  /
// 開発環境：Wandbox(online) https://wandbox.org/
// コンパイラー：gcc 13.2.0
// コマンド：g++ prog.cc -Wall -Wextra -std=c++17 -pedantic
// 説明：

#include <iostream>

// 今日からの日数カウントクラス
class FromToday {
// ここを作る
};

// メイン関数
// クラスの仕様検証
int main() {
    FromToday olympic("2021/7/23");
    olympic.Show();

    olympic.Set("S39/10/10");
    olympic.Show();

    FromToday earthQuake(2011, 3, 11);
    earthQuake.Show();

    earthQuake.Set('H', 7, 1, 17);

    FromToday hanshin = earthQuake;
    hanshin.Show();

    return 0;
}
