// 課題： pd10a 汎用関数 課題
// 氏名：下地慶英　学生証番号：24TB4039　日付：2026/07/05
// 開発環境：Wandbox(online) https://wandbox.org/
// コンパイラー：gcc 13.2.0
// コマンド：g++ prog.cc -Wall -Wextra -std=c++17 -pedantic
// 説明：
//   - 任意型の1次元配列を処理する汎用関数 max, median, reverse, find を実装した
//   - medianは配列を複製してソートし、要素数が偶数のときは(要素数/2)番目に大きい値を返す（元の配列は変更しない）
// 発展：
//   - 汎用関数 min と mean（平均値）を追加した

#include <iostream>
#include <stdlib.h>

// 汎用関数：配列の最大値を返す
template <class T>
T max(T* array, int n) {
    T m = array[0];
    for (int i = 1; i < n; i ++) {
        if (array[i] > m) m = array[i];
    }
    return m;
}

// 発展：配列の最小値を返す
template <class T>
T min(T* array, int n) {
    T m = array[0];
    for (int i = 1; i < n; i ++) {
        if (array[i] < m) m = array[i];
    }
    return m;
}

// 汎用関数：配列の中央値を返す（元の配列は変更しない）
template <class T>
T median(T* array, int n) {
    T* temp = new T[n];
    for (int i = 0; i < n; i ++) {
        temp[i] = array[i];
    }
    // 選択ソート（昇順）
    for (int i = 0; i < n - 1; i ++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j ++) {
            if (temp[j] < temp[minIndex]) minIndex = j;
        }
        T t = temp[i];
        temp[i] = temp[minIndex];
        temp[minIndex] = t;
    }
    T result = temp[n / 2];
    delete[] temp;
    return result;
}

// 発展：配列の平均値を返す（戻り値はdoubleに固定）
template <class T>
double mean(T* array, int n) {
    double sum = 0;
    for (int i = 0; i < n; i ++) {
        sum += (double)array[i];
    }
    return sum / n;
}

// 汎用関数：配列の内容を逆順に書き替える
template <class T>
void reverse(T* array, int n) {
    for (int i = 0; i < n / 2; i ++) {
        T t = array[i];
        array[i] = array[n - 1 - i];
        array[n - 1 - i] = t;
    }
}

// 汎用関数：検索値のインデックスを返す（見つからなければ-1）
template <class T>
int find(T* array, int n, T query) {
    for (int i = 0; i < n; i ++) {
        if (array[i] == query) return i;
    }
    return -1;
}

// テスト汎用関数
template <class T>
void Test(T* array, int n, T query) {
    std::cout << "array : ";
    for (int i = 0; i < n; i ++) {
        std::cout << array[i] << ", ";
    }
    std::cout << std::endl;
    std::cout << "max = " << max(array, n) << std::endl;
    std::cout << "min = " << min(array, n) << std::endl;
    std::cout << "median = " << median(array, n) << std::endl;
    std::cout << "mean = " << mean(array, n) << std::endl;
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
