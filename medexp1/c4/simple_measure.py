import subprocess
import csv
import os
import platform
import re
import datetime # 日時取得用

# ---------------------------------------------------------
# 設定：ここを変えるだけでNの上限を変更できます
MAX_N = 20
# ---------------------------------------------------------

def get_user_time(stderr_output):
    """OSごとの出力から実行時間(User time)だけを抜き出す関数"""
    system = platform.system()
    
    if system == 'Darwin':  # Macの場合
        # 出力例: "0.12 user"
        match = re.search(r'([\d\.]+)\s+user', stderr_output)
    else:  # Windows(WSL) / Linuxの場合
        # 出力例: "User time (seconds): 0.12"
        match = re.search(r'User time \(seconds\): ([\d\.]+)', stderr_output)
        
    if match:
        return match.group(1)
    return "Error"

def run():
    # ★ここが重要：実行時刻をファイル名に入れるので上書きされません
    now_str = datetime.datetime.now().strftime('%Y%m%d_%H%M%S')
    output_csv = f'simple_time_result_{now_str}.csv'
    
    src_file = 'kadai5q.c'  # 使うファイル

    # 計測する3つのパターン (コンパイルオプション, CSVヘッダ名)
    configs = [
        ([],       'k5'),
        (['-O2'],  'O2'),
        (['-O3'],  'O3')
    ]

    # timeコマンドの確認
    time_cmd_path = '/usr/bin/time'
    if not os.path.exists(time_cmd_path):
        print("エラー: /usr/bin/time が見つかりません。")
        return

    # OSに合わせてtimeコマンドのオプションを決定
    current_os = platform.system()
    if current_os == 'Darwin':
        base_time_cmd = [time_cmd_path, '-l']
    else:
        base_time_cmd = [time_cmd_path, '-v']

    print(f"計測を開始します (N=4 ～ {MAX_N})")
    print(f"結果保存先: {output_csv}\n")

    try:
        with open(output_csv, 'w', newline='') as f:
            writer = csv.writer(f)
            
            # ヘッダー書き込み: N, k5, O2, O3
            header = ['N'] + [c[1] for c in configs]
            writer.writerow(header)
            f.flush()

            for n in range(4, MAX_N + 1):
                times = []
                print(f"N={n}: ", end='', flush=True)

                for flags, name in configs:
                    # 1. コンパイル
                    # 誰かが同時に実行しても大丈夫なように PID をファイル名に付ける
                    pid = os.getpid()
                    exe_name = f'a_{pid}.out'
                    
                    compile_cmd = ['gcc', f'-DN={n}', src_file, '-lm', '-o', exe_name] + flags
                    try:
                        subprocess.run(compile_cmd, check=True)
                    except:
                        print("x", end=' ', flush=True)
                        times.append("Error")
                        continue

                    # 2. 実行 & 時間計測
                    # 画面が文字だらけにならないように stdout を捨てる
                    try:
                        proc = subprocess.run(
                            base_time_cmd + [f'./{exe_name}'],
                            stdout=subprocess.DEVNULL,
                            stderr=subprocess.PIPE,
                            text=True
                        )
                        # 3. 時間を抽出
                        t = get_user_time(proc.stderr)
                        times.append(t)
                        print(f"[{name}:{t}s] ", end='', flush=True)
                        
                    except KeyboardInterrupt:
                        if os.path.exists(exe_name): os.remove(exe_name)
                        raise

                    # 実行ファイル削除
                    if os.path.exists(exe_name):
                        os.remove(exe_name)

                # CSVに書き込み
                writer.writerow([n] + times)
                f.flush()
                print("")

        print(f"\n計測完了！結果は {output_csv} にあります。")

    except KeyboardInterrupt:
        print("\n\n中断しました。")
        print(f"ここまでのデータは {output_csv} に保存されています。")

if __name__ == "__main__":
    run()