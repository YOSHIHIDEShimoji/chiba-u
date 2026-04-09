#!/usr/bin/env zsh
set -euo pipefail

# 使い方:
#   ./2_run_plot.zsh <PARENT_DIR> [--color-vi <c>] [--color-vo <c>] [--color-add <c>] [--color-ch1 <c>] [--color-ch2 <c>] [--xlsx]
# 例:
#   ./2_run_plot.zsh analog2 --color-ch1 deepskyblue --color-ch2 orange
#   ./2_run_plot.zsh analog2 --color-ch1 deepskyblue --color-ch2 red --xlsx

if [[ $# -lt 1 ]]; then
  echo "Usage: $0 <PARENT_DIR> [--color-vi <c>] [--color-vo <c>] [--color-add <c>] [--color-ch1 <c>] [--color-ch2 <c>] [--xlsx]" >&2
  exit 1
fi

BASE_DIR="$1"
shift

source ~/MyProjects/python/.venv/bin/activate

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
    --color-ch1)
      PLOT_COLOR_ARGS+=("--color-ch1" "$2")
      shift 2
      ;;
    --color-ch2)
      PLOT_COLOR_ARGS+=("--color-ch2" "$2")
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

# コレで完了
python3 plot.py "$BASE_DIR" "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"

# ch1=V_o, ch2=V_{fo} じゃないなら以下を修正
python3 plot.py "$BASE_DIR/4_1" --ch1 V_o --ch2 V_{fo} "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"
python3 plot.py "$BASE_DIR/4_2" --ch1 V_o --ch2 V_{fo} "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"
python3 plot.py "$BASE_DIR/4_3" --ch1 V_o --ch2 V_{fo} "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"
python3 plot.py "$BASE_DIR/4_4" --ch1 V_o --ch2 V_{fo} "${XLSX_FLAG[@]}" "${PLOT_COLOR_ARGS[@]}"

# combined.png を収集（collect_combined.py の仕様に依存）
python3 collect_combined.py "$BASE_DIR"
