import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import os
import sys
import argparse

def generate_graphs():
    parser = argparse.ArgumentParser(description='Generate time graph from simple measure CSV.')
    parser.add_argument('csv_file', help='Path to the input CSV file (e.g., time_result_2025....csv)')
    args = parser.parse_args()

    input_csv = args.csv_file

    if not os.path.exists(input_csv):
        print(f"Error: ファイル '{input_csv}' が見つかりません。パスを確認してください。")
        return

    # --- フォルダ名の自動生成ロジック ---
    base_name = os.path.basename(input_csv)
    name_without_ext = os.path.splitext(base_name)[0]
    
    if name_without_ext.startswith("time_result_"):
        suffix = name_without_ext.replace("time_result_", "", 1)
    else:
        suffix = name_without_ext

    output_dir = f'simple_graphs_{suffix}'
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

    # 設定 (k5, O2, O3)
    scenarios = [
        {'key': 'k5', 'label': 'Standard (k5)',   'color': 'red',    'marker': 'o'},
        {'key': 'O2', 'label': 'O2 Opt',          'color': 'green',  'marker': '^'},
        {'key': 'O3', 'label': 'O3 Opt',          'color': 'orange', 'marker': 'D'},
    ]

    # --- 共通のフォーマット設定関数 ---
    def setup_plot(title, ylabel):
        plt.title(title, fontsize=14)
        plt.xlabel('N (Input Size)', fontsize=12)
        plt.ylabel(ylabel, fontsize=12)
        plt.grid(True, which='both', linestyle='--', alpha=0.7)
        
        if not df['N'].empty:
            plt.xticks(df['N'])

        ax = plt.gca()
        formatter = ticker.ScalarFormatter(useMathText=True)
        formatter.set_powerlimits((-2, 3)) 
        ax.yaxis.set_major_formatter(formatter)

    # --- 1. 全部のせグラフ (k5, O2, O3) ---
    def plot_time_all():
        filename = 'graph_time_all.png'
        plt.figure(figsize=(10, 6))
        
        has_data = False
        for sc in scenarios:
            col_name = sc['key']
            if col_name in df.columns:
                plt.plot(df['N'], df[col_name], 
                         label=sc['label'], 
                         color=sc['color'], 
                         marker=sc['marker'], 
                         markersize=5, alpha=0.8)
                has_data = True
        
        if has_data:
            setup_plot('Execution Time Comparison', 'Time [s]')
            plt.legend() # 凡例あり
            
            plt.tight_layout()
            save_path = os.path.join(output_dir, filename)
            plt.savefig(save_path, dpi=300)
            print(f"Saved: {save_path}")
        plt.close()

    # --- 2. Standardだけグラフ (凡例なし) ---
    def plot_time_standard():
        filename = 'graph_time_standard.png'
        plt.figure(figsize=(10, 6))
        
        # k5 (Standard) だけを探してプロット
        target_key = 'k5'
        # scenariosからk5の設定を探す
        sc = next((item for item in scenarios if item['key'] == target_key), None)
        
        if sc and target_key in df.columns:
            plt.plot(df['N'], df[target_key], 
                     color=sc['color'], 
                     marker=sc['marker'], 
                     markersize=5, alpha=0.8)
            
            setup_plot('Execution Time', 'Time [s]')
            # plt.legend() は呼ばない (凡例なし)
            
            plt.tight_layout()
            save_path = os.path.join(output_dir, filename)
            plt.savefig(save_path, dpi=300)
            print(f"Saved: {save_path}")
        else:
            print(f"Warning: Standard(k5)のデータがないため {filename} は作成しません。")
        plt.close()

    # --- 実行 ---
    plot_time_all()
    plot_time_standard()

    print("\nグラフ作成完了！")

if __name__ == "__main__":
    generate_graphs()