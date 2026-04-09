import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import os
import sys
import argparse

def generate_graphs():
    parser = argparse.ArgumentParser(description='Generate performance graphs from a CSV file.')
    parser.add_argument('csv_file', help='Path to the input CSV file (e.g., result_2025..._Linux.csv)')
    
    # 除外設定用の引数 (例: --no k5)
    parser.add_argument('--no', nargs='+', default=[], help='除外したいシナリオのキーを指定 (例: --no k5)')
    
    args = parser.parse_args()

    input_csv = args.csv_file

    if not os.path.exists(input_csv):
        print(f"Error: ファイル '{input_csv}' が見つかりません。パスを確認してください。")
        return

    # --- フォルダ名の自動生成 ---
    base_name = os.path.basename(input_csv)
    name_without_ext = os.path.splitext(base_name)[0]
    
    if name_without_ext.startswith("result_"):
        suffix = name_without_ext.replace("result_", "", 1)
    else:
        suffix = name_without_ext

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

    all_scenarios = [
        # {'key': 'k5',     'label': 'Standard (Printf)', 'color': 'gray',   'marker': 'o'},
        {'key': 'O0',     'label': 'O0',                'color': 'blue',   'marker': 's'},
        {'key': 'Os',     'label': 'Os',                'color': 'cyan',   'marker': 'v'},
        {'key': 'O2',     'label': 'O2',                'color': 'green',  'marker': '^'},
        {'key': 'O3',     'label': 'O3',                'color': 'orange', 'marker': 'D'},
        # {'key': 'native', 'label': 'Native (O3+march)', 'color': 'red',    'marker': 'x'},
    ]

    # 引数で指定されたキーを除外するフィルタリング
    if args.no:
        print(f"以下のシナリオを除外してプロットします: {args.no}")
        scenarios = [s for s in all_scenarios if s['key'] not in args.no]
    else:
        scenarios = all_scenarios

    if not scenarios:
        print("Error: すべてのシナリオが除外されました。グラフを作成できません。")
        return

    def plot_metric(suffix, title, ylabel, filename):
        plt.figure(figsize=(10, 6))
        
        has_data = False
        # シナリオごとにプロット
        for sc in scenarios:
            col_name = f"{sc['key']}_{suffix}"
            
            # CSVにその列が存在する場合のみプロット
            if col_name in df.columns:
                plt.plot(df['N'], df[col_name], 
                         label=sc['label'], 
                         color=sc['color'], 
                         marker=sc['marker'], 
                         markersize=5, 
                         alpha=0.8)
                has_data = True
        
        if not has_data:
            print(f"Warning: {filename} 用のデータ列がCSV内に見つかりませんでした。スキップします。")
            plt.close()
            return

        plt.title(title, fontsize=14)
        plt.xlabel('N', fontsize=12)
        plt.ylabel(ylabel, fontsize=12)
        plt.grid(True, which='both', linestyle='--', alpha=0.7)
        plt.legend()
        
        if not df['N'].empty:
            plt.xticks(df['N'])

        ax = plt.gca()
        # 軸の数値を指数表記など見やすくする
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