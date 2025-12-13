import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from scipy.stats import pearsonr, ttest_ind, linregress

# CSV ファイル名
INPUT_CSV = 'spotify_meta.csv'

# 出力ディレクトリ作成
os.makedirs('figure', exist_ok=True)

# データ読み込み
df = pd.read_csv(INPUT_CSV)

# Japan, Global, Both のデータフレーム作成
df_japan = df[df['country'] == 'Japan'].copy()
df_global = df[df['country'] == 'Global'].copy()
df_both = pd.concat([df_japan, df_global], ignore_index=True)

# 相関係数算出
def print_corr(df_both, df_japan, df_global):
    x_col = 'release_year'
    y_col = 'duration_ms'

    # 相関係数
    r_japan, _ = pearsonr(df_japan[x_col], df_japan[y_col])
    r_global, _ = pearsonr(df_global[x_col], df_global[y_col])
    r_both, _ = pearsonr(df_both[x_col], df_both[y_col])

    print('=== 曲の長さとリリース年の相関係数 ===')
    print(f'  {"r(Japan)":18s} = {r_japan:8.4f}')
    print(f'  {"r(Global)":18s} = {r_global:8.4f}')
    print(f'  {"r(Japan + Global)":18s} = {r_both:8.4f}')
    
# Japan + Global の線形回帰
def regression_both(df_both):
    x = df_both['release_year'].values
    y = df_both['duration_ms'].values

    result = linregress(x, y)

    print('\n=== 線形回帰 (Japan + Global) ===')
    print('duration_s = a * release_year + b としたとき、')
    print(f'  {"a":3s} = {result.slope/1000:11.3f} [s/year]')
    print(f'  {"b":3s} = {result.intercept/1000:11.3f} [s]')
    print(f'  {"r":3s} = {result.rvalue:11.3f}')
    print(f'  {"p":3s} = {result.pvalue:11.3e}')

# Japan の線形回帰
def regression_japan(df_japan):
    x = df_japan['release_year'].values
    y = df_japan['duration_ms'].values

    result = linregress(x, y)

    print('\n=== 線形回帰 (Japan) ===')
    print('duration_s = a * release_year + b としたとき、')
    print(f'  {"a":3s} = {result.slope/1000:11.3f} [s/year]')
    print(f'  {"b":3s} = {result.intercept/1000:11.3f} [s]')
    print(f'  {"r":3s} = {result.rvalue:11.3f}')
    print(f'  {"p":3s} = {result.pvalue:11.3e}')

# Japan と Global の t検定
def run_ttests(df_japan, df_global):
    feature_cols = [
        'duration_ms',
        'popularity',
        'artist_popularity',
        'artist_followers',
        'release_year',
    ]

    print('\n=== Japan と Global の平均値に差があるかのt検定 ===')
    for col in feature_cols:
        j = df_japan[col].values
        g = df_global[col].values

        t_stat, p_val = ttest_ind(j, g, equal_var=False)
        print(f'  {col:18s}: t = {t_stat:8.3f},  p = {p_val:8.3e}')

# Japan と Global の散布図
def save_scatter_both(df_both, df_japan, df_global):
    x_col = 'release_year'
    y_col = 'duration_ms'

    color_map = {
        'Japan': 'orange',
        'Global': 'deepskyblue',
    }

    plt.figure(figsize=(7, 4))

    for c in ['Global', 'Japan']:
        sub = df_both[df_both['country'] == c]
        x = sub[x_col].values
        y = sub[y_col].values / 1000.0  # ms を s に変換
        plt.scatter(x, y, label=c, color=color_map[c])

    plt.xlabel('release_year')
    plt.ylabel('duration [s]')
    plt.title('Relationship between release year and duration [s] (Global and Japan)')
    plt.ylim(100, 450)
    plt.grid(True)
    plt.legend(loc='upper left')

    plt.tight_layout()
    plt.savefig('figure/scatter_release_year_duration_relationship_global_japan.png', dpi=300)
    plt.close()

# Japan の散布図(線形近似)
def save_scatter_japan(df_japan):
    x_col = 'release_year'
    y_col = 'duration_ms'

    x = df_japan[x_col].values
    y_ms = df_japan[y_col].values
    y_s = y_ms / 1000.0

    result_ms = linregress(x, y_ms)
    slope_ms = result_ms.slope
    intercept_ms = result_ms.intercept

    x_lin = np.linspace(x.min(), x.max(), 100)
    y_lin_s = (slope_ms * x_lin + intercept_ms) / 1000.0  # ms を s に変換

    plt.figure(figsize=(7, 4))

    plt.scatter(x, y_s, label='Japan', color='orange')
    plt.plot(x_lin, y_lin_s, color='blue', linewidth=2)

    slope_s = slope_ms / 1000.0             # ms を s に変換
    intercept_s = intercept_ms / 1000.0     # ms を s に変換
    eq_text = f'y = {slope_s:.2f} x + {intercept_s:.1f}'
    plt.text(
        0.02,
        0.80,
        eq_text,
        transform=plt.gca().transAxes,
        fontsize=11,
        bbox=dict(facecolor='white', edgecolor='gray'),
    )

    plt.xlabel('release_year')
    plt.ylabel('duration [s]')
    plt.title('Relationship between release year and duration [s] (Japan)')
    plt.ylim(100, 450)
    plt.grid(True)
    plt.legend(loc='upper left')

    plt.tight_layout()
    plt.savefig('figure/scatter_release_year_duration_relationship_japan.png', dpi=300)
    plt.close()

# 箱ひげ図
def save_boxplot(df_both):
    sns.set_style(style='whitegrid')

    df_features = df_both.copy()

    exclude_cols = ['country', 'track_name', 'artist_name']
    numeric_cols = [
        c
        for c in df_features.columns
        if c not in exclude_cols and np.issubdtype(df_features[c].dtype, np.number)
    ]

    df_long = df_features[['country'] + numeric_cols].melt(
        id_vars='country',
        var_name='feature_name',
        value_name='value_raw',
    )

    # 標準化（平均0, 分散1）
    df_long['value'] = df_long.groupby('feature_name')['value_raw'].transform(
        lambda x: (x - x.mean()) / x.std(ddof=0)
    )

    plt.figure(figsize=(8, 4))
    ax = sns.boxplot(
        data=df_long,
        x='country',
        y='value',
        hue='feature_name',
        order=['Global', 'Japan'],
        showfliers=False,
    )

    plt.title('Comparison of the Top 50 Weekly Spotify Charts in Japan and Global')

    plt.xlabel('')
    plt.ylabel('value (standardized)')

    # 凡例
    handles, labels = ax.get_legend_handles_labels()
    plt.legend(
        handles,
        labels,
        title='',
        bbox_to_anchor=(1.02, 1),
        loc='upper left',
        borderaxespad=0.0,
    )

    plt.tight_layout()
    plt.savefig('figure/boxplot_global_japan.png', dpi=300)
    plt.close()

# メイン関数
if __name__ == '__main__':
    # 相関係数
    print_corr(df_both, df_japan, df_global)

    # 線形回帰 (global + Japan)
    regression_both(df_both)

    # 線形回帰（Japan)
    regression_japan(df_japan)

    # t検定
    run_ttests(df_japan, df_global)

    # 散布図 (global + Japan)
    save_scatter_both(df_both, df_japan, df_global)

    # 散布図 （Japan）
    save_scatter_japan(df_japan)

    # 箱ひげ図
    save_boxplot(df_both)

    # print
    print()
    print('saved: figure/scatter_release_year_duration_relationship_global_japan.png')
    print('saved: figure/scatter_release_year_duration_relationship_japan.png')
    print('saved: figure/boxplot_global_japan.png')