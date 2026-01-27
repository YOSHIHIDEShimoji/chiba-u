#!/bin/bash

# 設定
SRC="kadai5q-noprintf.c"
TRIALS=1000
BEST_FILE="result_loop.txt"      # 固定のファイル名に変更
EXE="./build/tsp_solver"

# コンパイル
mkdir -p ./build
gcc -O3 -DN=100 -DTRIALS=$TRIALS "$SRC" -o "$EXE" -lm

if [ $? -ne 0 ]; then
    echo "コンパイルエラー"
    exit 1
fi

# 過去の記録を引き継ぐ
current_best=99999999.9
total_loops=0

echo "Commencing search... (Press Ctrl+C to stop)"
if [ -f "$BEST_FILE" ]; then
    # ファイルからコストを読み取り
    val=$(grep "cost =" "$BEST_FILE" | awk -F'=' '{print $2}' | awk '{print $1}')
    if [ -n "$val" ]; then
        current_best=$val
    fi
    # ファイルから累計ループ数を読み取り
    last_loop=$(grep "Total Loops =" "$BEST_FILE" | awk -F'=' '{print $2}' | awk '{print $1}')
    if [ -n "$last_loop" ]; then
        total_loops=$last_loop
    fi
fi

# 無限ループ実行
while true
do
    total_loops=$((total_loops + 1))

    # Cプログラムを実行
    output=$("$EXE")
    line=$(echo "$output" | grep "cost =")
    new_cost=$(echo "$line" | awk -F'=' '{print $2}' | awk '{print $1}')

    if [ -n "$new_cost" ]; then
        # 今回の結果が過去ベストより良いか比較
        is_better=$(awk -v n="$new_cost" -v c="$current_best" 'BEGIN {print (n < c) ? 1 : 0}')

        if [ "$is_better" -eq 1 ]; then
            current_best=$new_cost
            
            # パスを 0始まり・ハイフン繋ぎ に整形
            formatted_path=$(echo "$line" | awk '{
                sub(/cost.*/, "");
                gsub(/[C>]/, " ");
                path = "";
                for(i=1; i<=NF-1; i++) {
                    val = $i - 1;
                    if(path == "") path = val;
                    else path = path "-" val;
                }
                print path;
            }')

            # 常に同じファイルに上書き保存
            {
                echo "Total Loops = $total_loops"
                echo "cost = $new_cost"
                echo "path = $formatted_path"
                echo "Last Updated: $(date '+%Y-%m-%d %H:%M:%S')"
            } > "$BEST_FILE"

            echo "[UPDATE] Loop $total_loops: Cost = $new_cost (Saved to $BEST_FILE)"
        fi
    fi
done