// 課題： pd07a 今日からの日数
// 氏名：下地慶英　学生証番号：24TB4039　日付：2026/06/15
// 開発環境：Wandbox(online) https://wandbox.org/
// コンパイラー：gcc 13.2.0
// コマンド：g++ prog.cc -Wall -Wextra -std=c++17 -pedantic

// 説明：
//       工夫した点：
//         - コンストラクタは対応するSet()を呼ぶだけにした
//         - sscanf()で文字列の先頭文字で西暦/和暦を自動判定した
//         - struct tm + mktime() + difftime() で日数差を計算した
// 調べた関数：
//   - mktime() : tm構造体をtime_tに変換する。年月日から時刻を求められる。
//   - difftime() : 2つのtime_tの差を秒単位で返す。

#include <iostream>
#include <ctime>
#include <cstdio>

class FromToday {
    int _year;
    int _month;
    int _day;

    // 和暦の元号と年数から西暦を返す
    int eraToYear(char era, int year) {
        switch (era) {
            case 'M': return 1868 + year - 1;
            case 'T': return 1912 + year - 1;
            case 'S': return 1926 + year - 1;
            case 'H': return 1989 + year - 1;
            case 'R': return 2019 + year - 1;
            default:  return year;
        }
    }

    // 文字列から日付を解析してメンバ変数にセットする
    // 先頭が数字なら西暦、アルファベットなら和暦として処理する
    void setDate(const char *str) {
        if (str[0] >= '0' && str[0] <= '9') {
            int y, m, d;
            sscanf(str, "%d/%d/%d", &y, &m, &d);
            _year = y; _month = m; _day = d;
        } else {
            char era;
            int y, m, d;
            sscanf(str, "%c%d/%d/%d", &era, &y, &m, &d);
            _year = eraToYear(era, y);
            _month = m; _day = d;
        }
    }

    // 設定した日付と今日の差分を日数で返す
    int getDays() {
        struct tm t = {};
        t.tm_year = _year - 1900;
        t.tm_mon  = _month - 1;
        t.tm_mday = _day;
        time_t target = mktime(&t);

        time_t now = time(NULL);
        struct tm todayTm = *localtime(&now);
        todayTm.tm_hour = 0;
        todayTm.tm_min  = 0;
        todayTm.tm_sec  = 0;
        time_t todayT = mktime(&todayTm);

        return (int)(difftime(target, todayT) / 86400.0 + 0.5);
    }

public:
    // コンストラクタ：対応するSet()を呼ぶ
    FromToday(const char *str)                         { Set(str); }
    FromToday(int year, int month, int day)            { Set(year, month, day); }
    FromToday(char era, int year, int month, int day)  { Set(era, year, month, day); }

    // Set：文字列で日付を設定（西暦または和暦）
    void Set(const char *str) {
        setDate(str);
    }

    // Set：数値（西暦）で日付を設定
    void Set(int year, int month, int day) {
        _year = year; 
        _month = month; 
        _day = day;
    }

    // Set：数値（和暦）で日付を設定
    void Set(char era, int year, int month, int day) {
        _year = eraToYear(era, year);
        _month = month; 
        _day = day;
    }

    // 今日からの日数を表示する
    void Show() {
        int days = getDays();
        // 今日
        if (days == 0) {
            std::cout << "今日です\n";
        // 未来
        } else if (days > 0) {
            if (days >= 365) {
                std::cout << days / 365 << "年" << days % 365 << "日後です\n";
            } else {
                std::cout << days << "日後です\n";
            }
        // 過去
        } else {
            days = -days;
            if (days >= 365) {
                std::cout << days / 365 << "年" << days % 365 << "日前です\n";
            } else {
                std::cout << days << "日前です\n";
            }
        }
    }
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
