import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import os
import sys
import argparse

def generate_graphs():
    parser = argparse.ArgumentParser(description='Generate performance graphs from a CSV file.')
    parser.add_argument('csv_file', help='Path to the input CSV file (e.g., result_2025..._Linux.csv)')
    args = parser.parse_args()

    input_csv = args.csv_file

    if not os.path.exists(input_csv):
        print(f"Error: ファイル '{input_csv}' が見つかりません。パスを確認してください。")
        return

    # --- フォルダ名の自動生成ロジック (変更) ---
    # 1. ファイル名(拡張子なし)を取得 -> "result_20251225_103000_Linux"
    base_name = os.path.basename(input_csv)
    name_without_ext = os.path.splitext(base_name)[0]
    
    # 2. 先頭の "result_" を削除して、残りをサフィックスとする
    # (例: "20251225_103000_Linux")
    if name_without_ext.startswith("result_"):
        suffix = name_without_ext.replace("result_", "", 1)
    else:
        suffix = name_without_ext

    # 3. フォルダ名を決定 -> "graphs_20251225_103000_Linux"
    output_dir = f'graphs_{suffix}'
    # ----------------------------------------

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
            print(f"Warning: {filename} 用のデータ列が見つかりませんでした。スキップします。")
            plt.close()
            return

        plt.title(title, fontsize=14)
        plt.xlabel('N (Input Size)', fontsize=12)
        plt.ylabel(ylabel, fontsize=12)
        plt.grid(True, which='both', linestyle='--', alpha=0.7)
        plt.legend()
        
        if not df['N'].empty:
            plt.xticks(df['N'])

        ax = plt.gca()
        formatter = ticker.ScalarFormatter(useMathText=True)
        formatter.set_powerlimits((-2, 3)) 
        ax.yaxis.set_major_formatter(formatter)

        plt.tight_layout()
        save_path = os.path.join(output_dir, filename)
        
        plt.savefig(save_path, dpi=300)
        
        plt.close()
        print(f"Saved: {save_path}")

    # --- グラフ生成実行 ---

    plot_metric('time[s]', 'Execution Time vs N', 'Time [s]', 'graph_time.png')
    plot_metric('mem[kb]', 'Memory Usage vs N', 'Memory [KB]', 'graph_memory.png')
    plot_metric('instr', 'CPU Instructions vs N', 'Instructions Retired', 'graph_instructions.png')

    print("\nすべてのグラフ作成が完了しました！")

if __name__ == "__main__":
    generate_graphs()