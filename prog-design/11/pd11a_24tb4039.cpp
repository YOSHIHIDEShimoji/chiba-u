// 課題： pd11a 巡回セールスマン問題
// 氏名：下地慶英　学生証番号：24TB4039　日付：2026/07/09
// 開発環境：Wandbox(online) https://wandbox.org/
// コンパイラー：gcc 13.2.0
// コマンド：g++ prog.cc -Wall -Wextra -std=c++17 -pedantic
// 説明：
//   - 都市データはcities100.csvを読み込みvector<City>に格納する
//   - 初期巡回路はstd::shuffleでランダムに決定し、2-opt法（最急降下法）で局所改善する
//   - 改善する2辺の組をすべて調べ、最も距離が縮む組をstd::reverseで反転して採用する
//   - これをN試行繰り返し、得られた巡回路をコストの昇順にstd::sortして上位5件を表示する
// 発展：
//   - 2-opt法の評価を巡回路全体の再計算ではなく、変化する2辺の距離差分のみで行い高速化した
//     （手元でN=200試行が約1.5秒で終わることを確認し、タイムアウトしない範囲で採用した）
// 調べた関数：
//   - std::shuffle（std::random_shuffleはC++17で廃止されたため代わりに使用）

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>

// 試行回数
const int TRIALS = 200;

// 都市の座標
struct City {
    int x, y;
};

// 1つの巡回路とその総移動距離
struct Tour {
    std::vector<int> order;
    double cost;
};

// cities100.csvを読み込む
std::vector<City> ReadCities(const char *filename) {
    std::vector<City> cities;
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Can't open data file.\n");
        exit(1);
    }
    char buf[256];
    while (fgets(buf, sizeof(buf), fp)) {
        int x, y;
        sscanf(buf, "%d,%d", &x, &y);
        cities.push_back({x, y});
    }
    fclose(fp);
    return cities;
}

// 2都市間の距離
double Distance(const City &a, const City &b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

// 巡回路の総移動距離を計算する
double TourCost(const std::vector<City> &cities, const std::vector<int> &order) {
    double cost = 0;
    int n = order.size();
    for (int i = 0; i < n; i ++) {
        cost += Distance(cities[order[i]], cities[order[(i + 1) % n]]);
    }
    return cost;
}

// 2-opt法による局所解探索（最急降下法）
// order[i+1..j]を反転すると辺(order[i],order[i+1])と(order[j],order[j+1])が
// 辺(order[i],order[j])と(order[i+1],order[j+1])に置き換わる。その差分だけで改善量を評価する
void SteepestDescent(const std::vector<City> &cities, Tour &tour) {
    int n = tour.order.size();
    while (true) {
        double best_diff = 0;
        int best_i = -1, best_j = -1;

        for (int i = 0; i < n - 1; i ++) {
            for (int j = i + 1; j < n; j ++) {
                const City &a = cities[tour.order[i]];
                const City &b = cities[tour.order[i + 1]];
                const City &c = cities[tour.order[j]];
                const City &d = cities[tour.order[(j + 1) % n]];
                double diff = (Distance(a, b) + Distance(c, d)) - (Distance(a, c) + Distance(b, d));
                if (diff > best_diff) {
                    best_diff = diff;
                    best_i = i;
                    best_j = j;
                }
            }
        }

        if (best_i == -1) break;
        std::reverse(tour.order.begin() + best_i + 1, tour.order.begin() + best_j + 1);
        tour.cost -= best_diff;
    }
}

// メイン関数
int main() {
    std::vector<City> cities = ReadCities("cities100.csv");
    int n = cities.size();

    std::random_device rd;
    std::mt19937 mt(rd());

    std::vector<Tour> results;
    for (int t = 0; t < TRIALS; t ++) {
        Tour tour;
        tour.order.resize(n);
        for (int i = 0; i < n; i ++) tour.order[i] = i;
        std::shuffle(tour.order.begin(), tour.order.end(), mt);
        tour.cost = TourCost(cities, tour.order);

        SteepestDescent(cities, tour);
        results.push_back(tour);
    }

    std::sort(results.begin(), results.end(),
        [](const Tour &a, const Tour &b) { return a.cost < b.cost; });

    printf("Number of trials: %d\n\n", TRIALS);
    for (int r = 0; r < 5; r ++) {
        printf("Rank %d  cost = %7.1f\n", r + 1, results[r].cost);
        for (int i = 0; i < n; i ++) {
            printf("C%-2d> ", results[r].order[i] + 1);
        }
        printf("C%-2d\n\n", results[r].order[0] + 1);
    }
}
