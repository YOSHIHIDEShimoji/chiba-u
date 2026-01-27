import subprocess
import re
import csv
import os
import sys
import time

# ==========================================
# 設定：完全「最適化なし」のガチンコ実験
# ==========================================
SOURCE_FILE = "kadai5q.c"
BUILD_DIR = "build"
EXECUTABLE = os.path.join(BUILD_DIR, "a.out")

# 結果保存ファイル名
FILE_TASK_A = "result_task_A_no_opt.csv"
FILE_TASK_B = "result_task_B_no_opt.csv"

# 【課題Aの設定】
# 都市数N vs CPU時間
# Nが増えると劇的に重くなる様子を観測する
N_LIST_A = list(range(4, 21)) + list(range(30, 101, 10))
TRIALS_A = 10000       # 1万回
OPT_LEVEL_A = "-O0"    # 最適化なし

# 【課題Bの設定】
# 試行回数 vs 最短距離
# 1万回粘って、最適化なしの純粋な探索推移を観測する
N_TASK_B = 100
TRIALS_B = 10000       # 1万回
OPT_LEVEL_B = "-O0"    # 最適化なし

# ==========================================
# 関数定義
# ==========================================

def compile_c(n, trials, optimize_level):
    """ 指定条件でコンパイル """
    if not os.path.exists(BUILD_DIR):
        os.makedirs(BUILD_DIR)
        
    cmd = [
        "gcc",
        f"-DN={n}",
        f"-DTRIALS={trials}",
        optimize_level,  # -O0
        SOURCE_FILE,
        "-lm",
        "-o", EXECUTABLE
    ]
    
    # コンパイル実行
    res = subprocess.run(cmd, capture_output=True, text=True)
    if res.returncode != 0:
        print(f"[Error] Compile failed at N={n}")
        print(res.stderr)
        sys.exit(1)

def run_and_measure_cpu():
    """ /usr/bin/time -p を使って CPU User Time を計測 """
    # Linux/MacなどUNIX系OS必須
    time_cmd = ["/usr/bin/time", "-p", EXECUTABLE]
    try:
        res = subprocess.run(time_cmd, capture_output=True, text=True)
    except FileNotFoundError:
        print("Error: /usr/bin/time が見つかりません。")
        sys.exit(1)

    # stderr から user時間を抽出
    cpu_time = 0.0
    for line in res.stderr.splitlines():
        if line.startswith("user"):
            parts = line.split()
            if len(parts) >= 2:
                cpu_time = float(parts[1])
                break
    return cpu_time, res.stdout

# ==========================================
# メイン処理
# ==========================================

def main():
    start_total = time.time()
    print("=== 実験開始 (最適化なし・高負荷モード) ===")
    print("※ 完了まで30分以上かかる可能性があります。PC電源に注意してください。")

    # ---------------------------------------------------------
    # 課題A: N vs 時間
    # ---------------------------------------------------------
    print(f"\n[課題A] Nを変化させて計測 (Trials={TRIALS_A}, Opt={OPT_LEVEL_A})")
    
    with open(FILE_TASK_A, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(["N", "CPU_User_Time_sec"])
        
        for n in N_LIST_A:
            # コンパイル
            compile_c(n, TRIALS_A, OPT_LEVEL_A)
            
            # 計測
            print(f"  Running N={n:<3} ... ", end="", flush=True)
            cpu_time, _ = run_and_measure_cpu()
            
            writer.writerow([n, cpu_time])
            print(f"{cpu_time:.2f} sec")

    print(f"-> 課題A完了。保存先: {FILE_TASK_A}")

    # ---------------------------------------------------------
    # 課題B: 収束グラフ (N=100, 10000回)
    # ---------------------------------------------------------
    print(f"\n[課題B] 収束推移を記録 (N={N_TASK_B}, Trials={TRIALS_B}, Opt={OPT_LEVEL_B})")
    print("  ※ N=100の最適化なし計算は非常に重いです。気長にお待ちください...")
    
    # コンパイル
    compile_c(N_TASK_B, TRIALS_B, OPT_LEVEL_B)
    
    # 実行
    _, output_text = run_and_measure_cpu()
    
    print("  -> ログ解析中...")
    
    global_min_cost = float('inf')
    lines = output_text.splitlines()
    current_trial_cost = None
    trial_count = 0
    
    with open(FILE_TASK_B, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(["Step", "Best_Cost_So_Far"])
        
        for line in lines:
            # "Trial X:" の行が来たら、前のTrialまでの集計を書き込み
            if "Trial" in line and ":" in line:
                if current_trial_cost is not None:
                    # 全体ベスト更新チェック
                    if current_trial_cost < global_min_cost:
                        global_min_cost = current_trial_cost
                    
                    # CSVに記録 (1~10000回すべて記録)
                    writer.writerow([trial_count, global_min_cost])
                
                trial_count += 1
                current_trial_cost = None
                
            # "cost =" の行から数値を拾う (そのTrial内の推移)
            if "cost =" in line:
                match = re.search(r"cost\s*=\s*([\d\.]+)", line)
                if match:
                    current_trial_cost = float(match.group(1))
        
        # 最後のTrial処理
        if current_trial_cost is not None:
            if current_trial_cost < global_min_cost:
                global_min_cost = current_trial_cost
            writer.writerow([trial_count, global_min_cost])

    end_total = time.time()
    print(f"-> 課題B完了。保存先: {FILE_TASK_B}")
    print(f"\n=== 全実験終了 (所要時間: {(end_total - start_total)/60:.1f} 分) ===")

if __name__ == "__main__":
    main()