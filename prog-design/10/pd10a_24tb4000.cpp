// 課題： pd10a 汎用関数 課題
// 氏名：　　　学生証番号：　　　日付：2026/  /
// 開発環境：Wandbox(online) https://wandbox.org/
// コンパイラー：gcc 13.2.0
// コマンド：g++ prog.cc -Wall -Wextra -std=c++17 -pedantic
// 説明：

#include <iostream>
#include <stdlib.h>

// テスト汎用関数
template <class T>
void Test(T* array, int n, T query) {
    std::cout << "array : ";
    for (int i = 0; i < n; i ++) {
        std::cout << array[i] << ", ";
    }
    std::cout << std::endl;
    std::cout << "max = " << max(array, n) << std::endl;
    std::cout << "median = " << median(array, n) << std::endl;
    reverse(array, n);
    std::cout << "reverse : ";
    for (int i = 0; i < n; i ++) {
        std::cout << array[i] << ", ";
    }
    std::cout << std::endl;
    std::cout << "find(" << query << ") = " << find(array, n, query) << std::endl;
}

// メイン関数
int main() {
    const int N = 10;

    // 整数配列の生成とテスト
    int array1[N];
    for (int i = 0; i < N; i ++) {
        array1[i] = rand() % N;
    }
    int query1 = array1[2];
    Test(array1, N, query1);

    // 倍精度実数配列の生成とテスト
    double array2[N];
    for (int i = 0; i < N; i ++) {
        array2[i] = rand() / (double)RAND_MAX;
    }
    double query2 = array2[3];
    Test(array2, N, query2);
}
