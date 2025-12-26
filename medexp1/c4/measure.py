import subprocess
import sys
import csv
import argparse
import os
import platform
import re
import datetime

def get_time_command():
    system = platform.system()
    if system == 'Darwin':
        return ['/usr/bin/time', '-l']
    else:
        return ['/usr/bin/time', '-v']

def parse_output(output, system_type):
    user_time = "N/A"
    max_memory_kb = "N/A"
    instructions = "N/A"

    if system_type == 'Darwin':
        match_time = re.search(r'([\d\.]+)\s+user', output)
        if match_time: user_time = match_time.group(1)
        match_mem = re.search(r'(\d+)\s+maximum resident set size', output)
        if match_mem: max_memory_kb = str(int(match_mem.group(1)) // 1024)
    else:
        match_time = re.search(r'User time \(seconds\): ([\d\.]+)', output)
        if match_time: user_time = match_time.group(1)
        match_mem = re.search(r'Maximum resident set size \(kbytes\): (\d+)', output)
        if match_mem: max_memory_kb = match_mem.group(1)

    match_instr_gnu = re.search(r'Instructions retired:\s*(\d+)', output, re.IGNORECASE)
    match_instr_bsd = re.search(r'(\d+)\s+instructions retired', output, re.IGNORECASE)

    if match_instr_gnu: instructions = match_instr_gnu.group(1)
    elif match_instr_bsd: instructions = match_instr_bsd.group(1)
    if instructions == '0': instructions = "N/A"

    return user_time, max_memory_kb, instructions

def run_experiment():
    parser = argparse.ArgumentParser(description='Measure execution time, memory, and instructions.')
    parser.add_argument('max_n', nargs='?', type=int, default=20, help='Maximum N value (default: 20)')
    args = parser.parse_args()

    start_n = 4
    end_n = args.max_n
    current_os = platform.system()

    # 結果保存用CSVファイル名
    now_str = datetime.datetime.now().strftime('%Y%m%d_%H%M%S')
    output_csv = f'result_{now_str}_{current_os}.csv'

    # ★修正ポイント: プロセスIDを使ってユニークな実行ファイル名を作る
    # これで他のターミナルで実行中のスクリプトとファイル名が被らなくなる
    pid = os.getpid()
    exe_file = f'./a_{pid}.out'

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
    print(f"PID: {pid} (実行ファイル: {exe_file})") # 確認用表示
    print(f"結果保存先: {output_csv}\n")

    try:
        with open(output_csv, 'w', newline='') as f:
            writer = csv.writer(f)
            
            header = ['N']
            header += [f"{c[2]}_time[s]" for c in configs]
            header += [f"{c[2]}_mem[kb]" for c in configs]
            header += [f"{c[2]}_instr"   for c in configs]
            
            writer.writerow(header)
            f.flush()

            for n in range(start_n, end_n + 1):
                times = []
                mems = []
                instrs = []
                
                print(f"N={n}: ", end='', flush=True)

                for src_file, extra_flags, prefix in configs:
                    # ★修正ポイント: -o オプションでユニークなファイル名を指定
                    compile_cmd = ['gcc', f'-DN={n}', src_file, '-lm', '-o', exe_file] + extra_flags
                    try:
                        subprocess.run(compile_cmd, check=True)
                    except subprocess.CalledProcessError:
                        print(f"[Error: {prefix}]", end=' ')
                        times.extend(["Error", "Error", "Error"])
                        continue

                    # ★修正ポイント: ユニークなファイルを実行
                    time_cmd = get_time_command() + [exe_file]
                    
                    try:
                        result = subprocess.run(
                            time_cmd, 
                            stdout=subprocess.DEVNULL, 
                            stderr=subprocess.PIPE, 
                            text=True
                        )
                    except KeyboardInterrupt:
                        raise

                    u_time, mem_kb, instr_val = parse_output(result.stderr, current_os)
                    
                    times.append(u_time)
                    mems.append(mem_kb)
                    instrs.append(instr_val)
                    
                    print(f"[{prefix}: {u_time}s] ", end='', flush=True)

                row = [n] + times + mems + instrs
                writer.writerow(row)
                f.flush()
                
                print("")

        print("\n計測完了！")

    except KeyboardInterrupt:
        print("\n\n計測を中断しました")
        print(f"データは {output_csv} に保存されています")
    
    except IOError as e:
        print(f"ファイルエラー: {e}")
    
    finally:
        # ★修正ポイント: 自分のPIDがついたファイルだけを削除する
        if os.path.exists(exe_file):
            os.remove(exe_file)

if __name__ == "__main__":
    run_experiment()