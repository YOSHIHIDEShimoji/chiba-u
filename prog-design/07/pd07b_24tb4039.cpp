// 課題： pd07b Matrixクラス
// 氏名：　　　学生証番号：　　　日付：2026/  /
// 開発環境：Wandbox(online) https://wandbox.org/
// コンパイラー：gcc 13.2.0
// コマンド：g++ prog.cc -Wall -Wextra -std=c++17 -pedantic
// 説明：

#include <iostream>

// 行列クラス
class Matrix {
// ここを作る
};

// メイン関数
// 行列クラスの動作検証を行う
int main()
{
	// 零行列コンストラクタと表示テスト
    Matrix mat1(2, 2);
    std::cout << "mat1=\n";
    mat1.Show();

	// 単一要素代入テスト
    mat1.Set(0, 0, 1.0);
    mat1.Set(0, 1, 2.0);
    mat1.Set(1, 0, 3.0);
    mat1.Set(1, 1, 4.0);
    std::cout << "mat1=\n";
    mat1.Show();

    // 単一要素取得テスト
    std::cout << "mat1(0,1)=" << mat1.Get(0,1) << "\n";

	// 要素指定コンストラクタのテスト
	double vals[4] = {5.1, 6.2, 7.3, 8.4};
    Matrix mat2(2, 2, vals);
    std::cout << "mat2=\n";
    mat2.Show();

    // 複製コンストラクタのテスト
    Matrix mat3(mat1);
    std::cout << "mat3=\n";
    mat3.Show();

    // 行列にスカラー値加算テスト
    mat3.Add(0.5);
    std::cout << "mat3=\n";
    mat3.Show();
    std::cout << "mat1=\n";
    mat1.Show();	// mat3 複製元のmat1は変化無いことを確認

	// 行列加算テスト
	mat3.Add(mat2);
    std::cout << "mat3=\n";
    mat3.Show();
    std::cout << "mat2=\n";
    mat2.Show();	// mat2は変化無いことを確認	
    
    // さらに独自の検証コードを追加するとよい
    // サイズの異なる行列同士を加算するとどうなるか，など

    return 0;
}
