#!/bin/sh
# 引数が2個なければエラー
if [ $# -ne 2 ]; then
    echo "Usage: $0 START_M END_M"
    exit 1
fi

start=$1
end=$2

# start は 1 以上
if [ "$start" -lt 1 ]; then
    echo "START_M($1) must be 1 or bigger"
    exit 1
fi

# end は start 以上
if [ "$end" -lt "$start" ]; then
    echo "END_M($2) must be START_M($1) or bigger"
    exit 1
fi

# M を start から end まで回す
for M in $(seq "$start" "$end"); do
    echo "== M = $M =="

    # コンパイラするファイルを適宜変更
    gcc -DM=$M c1_kadai4.c -o a.out

    ./a.out
    echo
done

# 実行方法
# chmod +x loop_m.sh
# ./loop_m.sh 3 10