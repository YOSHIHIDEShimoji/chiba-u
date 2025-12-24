import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import os
import sys

def generate_graphs():
    input_csv = 'result.csv'
    output_dir = 'graphs'

    if not os.path.exists(input_csv):
        print(f"Error: {input_csv} が見つかりません。")
        return

    try:
        df = pd.read_csv(input_csv)
    except Exception as e:
        print(f"Error: CSVの読み込みに失敗しました。 {e}")
        return

    os.makedirs(output_dir, exist_ok=True)
    print(f"フォルダ '{output_dir}' にグラフを作成します...")

    # データ前処理
    cols = df.columns.drop('N')
    for col in cols:
        df[col] = pd.to_numeric(df[col], errors='coerce')

    scenarios = [
        {'key': 'k5',        'label': 'Standard (k5)',   'color': 'red',    'marker': 'o'},
        {'key': 'nop',       'label': 'No-Printf',       'color': 'blue',   'marker': 's'},
        {'key': 'O2',        'label': 'O2 Opt',          'color': 'green',  'marker': '^'},
        {'key': 'O3',        'label': 'O3 Opt',          'color': 'orange', 'marker': 'D'},
        {'key': 'fast_math', 'label': 'Fast Math',       'color': 'purple', 'marker': 'x'},
    ]

    def plot_metric(suffix, title, ylabel, filename):
        plt.figure(figsize=(10, 6))
        
        has_data = False
        for sc in scenarios:
            col_name = f"{sc['key']}_{suffix}"
            if col_name in df.columns:
                plt.plot(df['N'], df[col_name], 
                         label=sc['label'], 
                         color=sc['color'], 
                         marker=sc['marker'], 
                         markersize=5, 
                         alpha=0.8)
                has_data = True
        
        if not has_data:
            print(f"Warning: {filename} 用のデータ列が見つかりませんでした。")
            plt.close()
            return

        plt.title(title, fontsize=14)
        plt.xlabel('N (Input Size)', fontsize=12)
        plt.ylabel(ylabel, fontsize=12)
        plt.grid(True, which='both', linestyle='--', alpha=0.7)
        plt.legend()
        plt.xticks(df['N'])

        # y軸のフォーマット設定
        ax = plt.gca()
        # useMathText=True で "1e7" を "x 10^7" に変換
        formatter = ticker.ScalarFormatter(useMathText=True)
        # 0.01未満 または 1000以上 の場合に 10^n 表記を使う
        formatter.set_powerlimits((-2, 3)) 
        ax.yaxis.set_major_formatter(formatter)

        plt.tight_layout()
        save_path = os.path.join(output_dir, filename)
        
        # 高解像度保存
        plt.savefig(save_path, dpi=300)
        
        plt.close()
        print(f"Saved: {save_path}")

    # --- グラフ生成実行 (線形スケールのみ) ---

    # 1. 時間
    plot_metric('time[s]', 'Execution Time vs N', 'Time [s]', 'graph_time.png')

    # 2. メモリ
    plot_metric('mem[kb]', 'Memory Usage vs N', 'Memory [KB]', 'graph_memory.png')

    # 3. 命令数
    plot_metric('instr', 'CPU Instructions vs N', 'Instructions Retired', 'graph_instructions.png')

    print("\nすべてのグラフ作成が完了しました！")

if __name__ == "__main__":
    generate_graphs()