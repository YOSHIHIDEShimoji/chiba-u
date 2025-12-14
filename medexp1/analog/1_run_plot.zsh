#!/usr/bin/env zsh
set -euo pipefail

# 使い方:
#   ./1_run_plot.zsh <PARENT_DIR> [--color-vi <c>] [--color-vo <c>] [--color-add <c>] [--xlsx]
#   ./1_run_plot.zsh analog1 --color-vi orange --color-vo deepskyblue --color-add red


if [[ $# -lt 1 ]]; then
  echo "Usage: $0 <PARENT_DIR> [--color-vi <c>] [--color-vo <c>] [--color-add <c>] [--xlsx]" >&2
  exit 1
fi

# 親ディレクトリ（例: analog1）
BASE_DIR="$1"
shift

# Python 仮想環境を有効化
source ~/MyProjects/python/.venv/bin/activate

# plot.py にそのまま渡すオプションをまとめる
PLOT_COLOR_ARGS=()
XLSX_FLAG=()

while [[ $# -gt 0 ]]; do
  case "$1" in
    --color-vi)
      PLOT_COLOR_ARGS+=("--color-vi" "$2")
      shift 2
      ;;
    --color-vo)
      PLOT_COLOR_ARGS+=("--color-vo" "$2")
      shift 2
      ;;
    --color-add)
      PLOT_COLOR_ARGS+=("--color-add" "$2")
      shift 2
      ;;
    --xlsx)
      XLSX_FLAG+=("--xlsx")
      shift 1
      ;;
    *)
      echo "Unknown option: $1" >&2
      exit 1
      ;;
  esac
done

# セッション配下を一括処理（combined.png を作るだけ）
python3 plot.py "$BASE_DIR" "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"

# 4_1_4
# CH1=Vi+, CH2=Vo, Vi- は定数
python3 plot.py "$BASE_DIR/4_1_4_-3" --ch1 V_{i+} --ch2 V_o --const-label V_{i-} --const-value -3 "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"
python3 plot.py "$BASE_DIR/4_1_4_0"  --ch1 V_{i+} --ch2 V_o --const-label V_{i-} --const-value 0  "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"
python3 plot.py "$BASE_DIR/4_1_4_2"  --ch1 V_{i+} --ch2 V_o --const-label V_{i-} --const-value 2  "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"

# Vi- を CSV で取得している場合は const-value を使わず、以下を実行。ただしファイル名(F0000CH3.CSV)に注意
# python3 plot.py "$BASE_DIR/4_1_4_-3" --ch1 V_{i+} --ch2 V_o --add-label V_{i-} --add-file F0000CH3.CSV "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"
# python3 plot.py "$BASE_DIR/4_1_4_0" --ch1 V_{i+} --ch2 V_o --add-label V_{i-} --add-file F0000CH3.CSV "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"
# python3 plot.py "$BASE_DIR/4_1_4_2" --ch1 V_{i+} --ch2 V_o --add-label V_{i-} --add-file F0000CH3.CSV "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"


# 4_2_4
# CH1=Vi, CH2=Vo
python3 plot.py "$BASE_DIR/4_2_4" --ch1 V_i --ch2 V_o "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"


# 4_3_5
# CH1=Vi, CH2=Vo
python3 plot.py "$BASE_DIR/4_3_5" --ch1 V_i --ch2 V_o "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"


# 4_4_7
# CH1=Vi, CH2=Vo
python3 plot.py "$BASE_DIR/4_4_7" --ch1 V_i --ch2 V_o "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"


# 4_5_4
# CH1=Vi1, CH2=Vo, Vi2 は定数
python3 plot.py "$BASE_DIR/4_5_4" --ch1 V_{i1} --ch2 V_o --const-label V_{i2} --const-value -1 "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"

# Vi2 を CSV で取得している場合は const-value を使わず、以下を実行。ただしファイル名(F0000CH3.CSV)に注意
# python3 plot.py "$BASE_DIR/4_5_4" --ch1 V_{i1} --ch2 V_o --add-label V_{i2} --add-file F0000CH3.CSV "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"


# 4_5_5
# CH1=Vi1（Vi2は同一入力なので凡例として追加）, CH2=Vo
python3 plot.py "$BASE_DIR/4_5_5" --ch1 V_{i1} V_{i2} --ch2 V_o "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"

# Vi2 を CSV で取得している場合は、以下を実行。ただしファイル名(F0000CH3.CSV)に注意
# python3 plot.py "$BASE_DIR/4_5_5" --ch1 V_{i1} --ch2 V_o --add-label V_{i2} --add-file F0000CH3.CSV "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"


# 4_6_4
# CH1=Vi1, CH2=Vo, Vi2 は定数
python3 plot.py "$BASE_DIR/4_6_4" --ch1 V_{i1} --ch2 V_o --const-label V_{i2} --const-value -1 "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"

# Vi2 を CSV で取得している場合は const-value を使わず、以下を実行。ただしファイル名(F0000CH3.CSV)に注意
# python3 plot.py "$BASE_DIR/4_6_4" --ch1 V_{i1} --ch2 V_o --add-label V_{i2} --add-file F0000CH3.CSV "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"


# 4_6_5
# CH1=Vi1, CH2=Vo, Vi2 は定数
python3 plot.py "$BASE_DIR/4_6_5" --ch1 V_{i1} --ch2 V_o --const-label V_{i2} --const-value 1 "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"

# Vi2 を CSV で取得している場合は const-value を使わず、以下を実行。ただしファイル名(F0000CH3.CSV)に注意
# python3 plot.py "$BASE_DIR/4_6_5" --ch1 V_{i1} --ch2 V_o --add-label V_{i2} --add-file F0000CH3.CSV "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"


# combined.png を収集（collect_combined.py の仕様に依存）
python3 collect_combined.py "$BASE_DIR"
