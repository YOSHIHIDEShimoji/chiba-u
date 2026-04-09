#!/bin/sh

# 実行するファイル
SRC_FILE="c2_kadai4.c"

# 引数が2個なければエラー
if [ $# -ne 2 ]; then
    echo "Usage: $0 START_K END_K"
    exit 1
fi

start=$1
end=$2

# start は 0 以上
if [ "$start" -lt 0 ]; then
    echo "START_K($1) must be 1 or bigger"
    exit 1
fi

# end は start 以上
if [ "$end" -lt "$start" ]; then
    echo "END_K($2) must be START_K($1) or bigger"
    exit 1
fi

# コンパイル
gcc "$SRC_FILE" -o a.out

# K を start から end まで回す
for K in $(seq "$start" "$end"); do
    echo "== K = $K =="
    ./a.out $K
    echo
done

# 実行方法
# loop_k.sh というファイル名で保存して以下を実行
# chmod +x loop_k.sh
# ./loop_k.sh 3 10
