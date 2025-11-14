#!/bin/zsh

source ~/.zshrc

maxN=""

while getopts "M:" opt; do
  case "$opt" in
    M)
      maxN=$OPTARG
      ;;
    *)
      echo "Usage: $0 [-M maxN] <source.c> <csv_dir>"
      exit 1
      ;;
  esac
done

shift $((OPTIND - 1))

exec_file=$1
csv_dir=$2

if [ -z "$exec_file" ] || [ -z "$csv_dir" ]; then
  echo "Usage: $0 [-M maxN] <source.c> <csv_dir>"
  exit 1
fi

setopt null_glob

nums=()
for f in "$csv_dir"/cities*.csv; do
  fname=${f:t}
  num=${fname#cities}
  num=${num%.csv}
  num=$((10#$num))

  if [ -n "$maxN" ] && [ "$num" -gt "$maxN" ]; then
    continue
  fi

  nums+=$num
done

if [ ${#nums} -eq 0 ]; then
  if [ -n "$maxN" ]; then
    echo "No cities*.csv files with N <= $maxN found in $csv_dir"
  else
    echo "No cities*.csv files found in $csv_dir"
  fi
  exit 1
fi

sorted_nums=("${(@on)nums}")

for n in $sorted_nums; do
  echo "===== N=$n ====="
  c -p -DN=$n "$exec_file"
  echo
done
