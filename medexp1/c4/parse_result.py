import re
import csv
import argparse
from pathlib import Path

def parse_args():
    parser = argparse.ArgumentParser(
        description="Parse TSP result.txt and output CSV with N, real, user, sys"
    )
    parser.add_argument(
        "input_file",
        help="解析する result.txt のパス（必須）"
    )
    parser.add_argument(
        "-o", "--output",
        default="_ignore/result.csv",
        help="出力CSVファイルのパス（任意, default: ./_ignore/result.csv）"
    )
    return parser.parse_args()

re_n = re.compile(r"^===== N=(\d+)\s*=====")
re_time = re.compile(r"^\s*(real|user|sys)\s+([\d.]+)")

def main():
    args = parse_args()

    input_path = Path(args.input_file)
    output_path = Path(args.output)

    if not input_path.exists():
        raise FileNotFoundError(f"{input_path} が見つかりません")

    rows = []
    current_n = None
    in_time_block = False
    time_values = {}

    with input_path.open("r", encoding="utf-8") as f:
        for line in f:
            line = line.rstrip("\n")

            m_n = re_n.match(line)
            if m_n:
                current_n = int(m_n.group(1))
                in_time_block = False
                time_values = {}
                continue

            if "TIME RESULT" in line:
                in_time_block = True
                time_values = {}
                continue

            if in_time_block:
                m_t = re_time.match(line)
                if not m_t:
                    continue

                key = m_t.group(1)
                val = float(m_t.group(2))
                time_values[key] = val

                if len(time_values) == 3 and current_n is not None:
                    rows.append({
                        "N": current_n,
                        "real": time_values.get("real", 0.0),
                        "user": time_values.get("user", 0.0),
                        "sys": time_values.get("sys", 0.0),
                    })
                    in_time_block = False
                    time_values = {}
                    continue

    rows.sort(key=lambda r: r["N"])

    output_path.parent.mkdir(parents=True, exist_ok=True)
    with output_path.open("w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(["N", "real", "user", "sys"])
        for r in rows:
            writer.writerow([
                r["N"],
                f"{r['real']:.2f}",
                f"{r['user']:.2f}",
                f"{r['sys']:.2f}",
            ])

    print(f"CSV 出力完了: {output_path}")

if __name__ == "__main__":
    main()
