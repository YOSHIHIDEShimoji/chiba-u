// 課題： pd11b テキスト解析
// 氏名：下地慶英　学生証番号：24TB4039　日付：2026/07/09
// 開発環境：Wandbox(online) https://wandbox.org/
// コンパイラー：gcc 13.2.0
// コマンド：g++ prog.cc -Wall -Wextra -std=c++17 -pedantic
// 説明：
//   - JobsSpeech.txtを解析し、出現頻度の高い単語トップ30を表示する
//   - 英字だけで単語を構成し、それ以外の記号は区切りとして無視する。大文字・小文字は区別しない
//   - 冠詞(a, an, the)とbe動詞(am, is, are, was, were, be, been, being)はカウントしない
//   - 単語の出現回数はmapクラス(連想配列)で数え、vectorに移してからstd::sortで頻度順に並べる

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>

const int TOP_N = 30;

// カウントしない単語（冠詞・be動詞）
const std::set<std::string> STOP_WORDS = {
    "a", "an", "the",
    "am", "is", "are", "was", "were", "be", "been", "being"
};

// テキストファイルを解析し、単語ごとの出現回数を数える
std::map<std::string, int> CountWords(const char *filename) {
    std::map<std::string, int> freq;
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Can't open data file.\n");
        exit(1);
    }

    std::string word;
    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (isalpha(c)) {
            word += (char)tolower(c);
        } else if (!word.empty()) {
            if (STOP_WORDS.count(word) == 0) freq[word] ++;
            word.clear();
        }
    }
    if (!word.empty() && STOP_WORDS.count(word) == 0) freq[word] ++;
    fclose(fp);

    return freq;
}

// メイン関数
int main() {
    std::map<std::string, int> freq = CountWords("JobsSpeech.txt");

    // mapをvectorに移して頻度の降順にソート
    std::vector<std::pair<std::string, int>> words(freq.begin(), freq.end());
    std::sort(words.begin(), words.end(),
        [](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
            return a.second > b.second;
        });

    printf("Top %d words:\n", TOP_N);
    for (int i = 0; i < TOP_N && i < (int)words.size(); i ++) {
        printf("%2d: %-12s %d\n", i + 1, words[i].first.c_str(), words[i].second);
    }
}
