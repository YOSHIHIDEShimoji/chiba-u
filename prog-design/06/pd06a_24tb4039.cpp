// 課題：pd06a ログ管理クラス
// 氏名：下地慶英　学生証番号：24tb4039　日付：2026/06/03
// 開発環境：Wandbox(online) https://wandbox.org/
// コンパイラー：gcc 13.2.0
// コマンド：g++ prog.cc -Wall -Wextra -std=c++17 -pedantic
// 説明：Loggerクラスを実装した。
//       発展として以下に対応した。
//         - int型・double型のOutput()オーバーロード
//         - タイムスタンプをログに付加
//         - Printf()で書式指定出力（可変引数）
// 調べた関数：
//   - ctime() : time()の戻り値を日時の文字列に変換する。末尾に\nがつく。
//   - vsnprintf() : va_listを使ってprintf形式で文字列をバッファに書き込む。
//   - va_list, va_start, va_end : 可変引数を扱うためのマクロ。

#include <cstdio>
#include <ctime>
#include <cstdarg>
#include <iostream>
#include <fstream>
#include <string>

class Logger {
    std::ofstream fp;

    // タイムスタンプをファイルに書き込む
    void WriteTimestamp() {
        auto t = time(NULL);
        char *ts = ctime(&t);
        ts[24] = '\0';
        fp << "[" << ts << "] ";
    }

public:
    // コンストラクタ：ファイルを開く
    Logger(const char *filename) {
        fp.open(filename);
        if (!fp) {
            std::cout << "Logger error: cannot open file" << std::endl;
        }
    }

    // デストラクタ：ファイルを閉じる
    ~Logger() {
        if (fp.is_open()) {
            fp.close();
        }
    }

    // 文字列をログファイルに書き込む
    void Output(const char *str) {
        if (!fp.is_open()) return;
        WriteTimestamp();
        fp << str << "\n";
    }

    // int型をログファイルに書き込む
    void Output(int value) {
        if (!fp.is_open()) return;
        WriteTimestamp();
        fp << value << "\n";
    }

    // double型をログファイルに書き込む
    void Output(double value) {
        if (!fp.is_open()) return;
        WriteTimestamp();
        fp << value << "\n";
    }

    // printf形式の書式指定でログファイルに書き込む
    void Printf(const char *fmt, ...) {
        if (!fp.is_open()) return;
        WriteTimestamp();
        char buf[256];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
        fp << buf;
    }
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
