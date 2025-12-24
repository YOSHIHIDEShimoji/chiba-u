import subprocess
import sys
import csv
import argparse
import os
import platform
import re

def get_time_command():
    """OSに応じて適切なtimeコマンドとオプションを返す"""
    system = platform.system()
    if system == 'Darwin':  # macOS
        return ['/usr/bin/time', '-l']
    else:  # Linux / WSL
        return ['/usr/bin/time', '-v']

def parse_output(output, system_type):
    """
    標準エラー出力から user時間, メモリ, 命令数 を抽出する
    """
    user_time = "N/A"
    max_memory_kb = "N/A"
    instructions = "N/A"

    # --- 1. User Time & Memory (OS別) ---
    if system_type == 'Darwin':  # macOS (-l)
        # Time
        match_time = re.search(r'([\d\.]+)\s+user', output)
        if match_time:
            user_time = match_time.group(1)
        # Memory (bytes -> KB)
        match_mem = re.search(r'(\d+)\s+maximum resident set size', output)
        if match_mem:
            max_memory_kb = str(int(match_mem.group(1)) // 1024)

    else:  # Linux / WSL (-v)
        # Time
        match_time = re.search(r'User time \(seconds\): ([\d\.]+)', output)
        if match_time:
            user_time = match_time.group(1)
        # Memory (kbytes)
        match_mem = re.search(r'Maximum resident set size \(kbytes\): (\d+)', output)
        if match_mem:
            max_memory_kb = match_mem.group(1)

    # --- 2. Instructions (形式に関わらず共通検索) ---
    # Linux/GNU形式: "Instructions retired: 12345"
    # BSD/Mac形式: "12345  instructions retired"
    
    # パターンA: 値が後ろにある場合 (GNU/WSL)
    match_instr_gnu = re.search(r'Instructions retired:\s*(\d+)', output, re.IGNORECASE)
    # パターンB: 値が前にある場合 (BSD/Mac)
    match_instr_bsd = re.search(r'(\d+)\s+instructions retired', output, re.IGNORECASE)

    if match_instr_gnu:
        instructions = match_instr_gnu.group(1)
    elif match_instr_bsd:
        instructions = match_instr_bsd.group(1)
    
    # Macなどで取得できず0の場合のケア（必要なら）
    if instructions == '0':
        instructions = "N/A"

    return user_time, max_memory_kb, instructions

def run_experiment():
    parser = argparse.ArgumentParser(description='Measure execution time, memory, and instructions.')
    parser.add_argument('max_n', nargs='?', type=int, default=20, help='Maximum N value (default: 20)')
    args = parser.parse_args()

    start_n = 4
    end_n = args.max_n
    output_csv = 'result.csv'  # ファイル名を指定通り変更
    current_os = platform.system()

    configs = [
        ('kadai5q.c',          [],                 'k5'),
        ('kadai5q-noprintf.c', [],                 'nop'),
        ('kadai5q-noprintf.c', ['-O2'],            'O2'),
        ('kadai5q-noprintf.c', ['-O3'],            'O3'),
        ('kadai5q-noprintf.c', ['-O3', '-ffast-math'], 'fast_math')
    ]
    
    if not os.path.exists('/usr/bin/time'):
        print("Error: /usr/bin/time が見つかりません。", file=sys.stderr)
        return

    print(f"環境: {current_os}")
    print(f"計測を開始します (N={start_n} ～ {end_n})")
    print(f"途中終了: Ctrl + C (データは保存されます)")
    print(f"結果保存先: {output_csv}\n")

    try:
        with open(output_csv, 'w', newline='') as f:
            writer = csv.writer(f)
            
            # ヘッダー作成: N, [time系], [mem系], [instr系]
            header = ['N']
            header += [f"{c[2]}_time[s]" for c in configs]
            header += [f"{c[2]}_mem[kb]" for c in configs]
            header += [f"{c[2]}_instr"   for c in configs]  # 追加
            
            writer.writerow(header)
            f.flush()

            for n in range(start_n, end_n + 1):
                times = []
                mems = []
                instrs = []
                
                print(f"N={n}: ", end='', flush=True)

                for src_file, extra_flags, prefix in configs:
                    compile_cmd = ['gcc', f'-DN={n}', src_file, '-lm'] + extra_flags
                    try:
                        subprocess.run(compile_cmd, check=True)
                    except subprocess.CalledProcessError:
                        print(f"[Error: {prefix}]", end=' ')
                        times.append("Error")
                        mems.append("Error")
                        instrs.append("Error")
                        continue

                    time_cmd = get_time_command() + ['./a.out']
                    
                    try:
                        result = subprocess.run(time_cmd, capture_output=True, text=True)
                    except KeyboardInterrupt:
                        raise

                    u_time, mem_kb, instr_val = parse_output(result.stderr, current_os)
                    
                    times.append(u_time)
                    mems.append(mem_kb)
                    instrs.append(instr_val)
                    
                    # 画面表示には時間だけ出す（スッキリさせるため）
                    print(f"[{prefix}: {u_time}s] ", end='', flush=True)

                # 行データ結合: N + time列 + mem列 + instr列
                row = [n] + times + mems + instrs
                writer.writerow(row)
                f.flush()
                
                print("")

        print("\n計測完了！")

    except KeyboardInterrupt:
        print("\n\n計測を中断しました (Ctrl + C)。")
        print(f"データは {output_csv} に保存されています。")
    
    except IOError as e:
        print(f"ファイルエラー: {e}")
    
    finally:
        if os.path.exists('./a.out'):
            os.remove('./a.out')

if __name__ == "__main__":
    run_experiment()