// 課題： pd06a ログ管理クラス
// 氏名：　　　学生証番号：　　　日付：2026/  /
// 開発環境：Wandbox(online) https://wandbox.org/
// コンパイラー：gcc 13.2.0
// コマンド：g++ prog.cc -Wall -Wextra -std=c++17 -pedantic
// 説明：


#include <iostream>

class Logger {
// ここを作る
};


// 処理部
// 何らかの処理を想定した関数
// for文の空ループは時間を浪費するだけの擬似コード（深い意味はない）
void Process() {
    Logger log("log.txt");

    log.Output("start program");        // 処理開始をログ記録

    for (int i = 0; i < 100000; i++);   // 何らかの処理を想定した時間浪費
    
    log.Output("end program");          // 処理終了をログ記録
}

// ログ表示
// Loggerクラスで出力された log.txt ファイルを開いて画面に表示する
void ShowLog() {
    std::ifstream logFile("log.txt");
    if (logFile) {
        std::string buffer;
        while(getline(logFile, buffer)) {
            std::cout << buffer << std::endl;
        }
    }
}

// 動作検証（テスト）プログラム
int main()
{
    Process();
    ShowLog();
    
    return 0;
}
