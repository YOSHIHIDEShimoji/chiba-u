import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from scipy.stats import pearsonr, ttest_ind, linregress

# CSV ファイル名
INPUT_CSV = "spotify_meta.csv"

# 出力ディレクトリ作成
SAVE_DIR = "figure"

os.makedirs(SAVE_DIR, exist_ok=True)

# データ読み込み
df = pd.read_csv(INPUT_CSV)

# Japan, Global, Both のデータフレーム作成
df_japan = df[df["country"] == "Japan"].copy()
df_global = df[df["country"] == "Global"].copy()
df_both = pd.concat([df_japan, df_global], ignore_index=True)

# Japan と Global の散布図を作成 
def save_scatter_release_vs_duration_both(df_all, df_japan, df_global, save_dir):
    x_col = "release_year"
    y_col = "duration_ms"

    plt.figure(figsize=(6, 4))

    color_map = {
        "Japan": "orange",
        "Global": "deepskyblue",
    }

    # 散布図（縦軸は秒に変換して描画）
    for c in ["Global", "Japan"]:
        sub_c = df_all[df_all["country"] == c].dropna(subset=[x_col, y_col])
        if len(sub_c) < 2:
            continue
        x = sub_c[x_col].values
        y_s = sub_c[y_col].values / 1000.0  # ms → s
        plt.scatter(x, y_s, alpha=0.7, label=c, color=color_map[c])

    # 相関係数はターミナル出力のみ
    def safe_corr(df_sub):
        sub = df_sub.dropna(subset=[x_col, y_col])
        if len(sub) < 2:
            return np.nan, np.nan
        return pearsonr(sub[x_col], sub[y_col])

    r_japan, _ = safe_corr(df_japan)
    r_global, _ = safe_corr(df_global)
    r_both, _ = safe_corr(df_all)

    print("\n======================================")
    print("[BOTH] duration_ms vs release_year")
    print(f"  r(Japan)  = {r_japan:.4f}")
    print(f"  r(Global) = {r_global:.4f}")
    print(f"  r(Both)   = {r_both:.4f}")
    print("======================================")

    plt.xlabel("release_year")
    plt.ylabel("duration [s]")  # 単位を秒で表示
    plt.title("duration [s] vs release_year (Japan & Global)")
    plt.grid(True)
    plt.legend(loc="upper left")

    filepath = os.path.join(save_dir, "scatter_duration_s_vs_release_year_both.png")
    plt.tight_layout()
    plt.savefig(filepath)
    plt.close()

    print(f"Saved scatter (Both): {filepath}")


# =========================================================
# 1-2. 散布図（Japan only）：release_year vs duration（秒）
#      ＋ 赤色の線形近似直線
#      相関係数はターミナルのみ
#      式 y = ax + b を左下に表示
# =========================================================
def save_scatter_release_vs_duration_japan(df_japan, save_dir):
    x_col = "release_year"
    y_col = "duration_ms"

    sub = df_japan.dropna(subset=[x_col, y_col])
    x = sub[x_col].values
    y_ms = sub[y_col].values
    y_s = y_ms / 1000.0  # プロット用に秒に変換

    # 回帰直線は ms 単位で計算
    result_ms = linregress(x, y_ms)
    slope_ms = result_ms.slope          # [ms / year]
    intercept_ms = result_ms.intercept  # [ms]

    # プロット用の直線は秒に変換して描く
    x_lin = np.linspace(x.min(), x.max(), 100)
    y_lin_s = (slope_ms * x_lin + intercept_ms) / 1000.0  # ms → s

    plt.figure(figsize=(6, 4))

    # 散布図（Japan, 縦軸は秒）
    plt.scatter(x, y_s, alpha=0.7, label="Japan", color="tab:orange")

    # 回帰直線（赤, 秒単位）
    plt.plot(x_lin, y_lin_s, color="red", linewidth=2, label="Linear fit")

    # 式を左下に秒単位で表示
    slope_s = slope_ms / 1000.0
    intercept_s = intercept_ms / 1000.0
    eq_text = f"y = {slope_s:.2f} x + {intercept_s:.1f}"
    plt.text(
        0.05,
        0.05,  # 左下
        eq_text,
        transform=plt.gca().transAxes,
        fontsize=11,
        bbox=dict(facecolor="white", alpha=0.7, edgecolor="gray"),
    )

    # 相関係数は ms で計算（s でも同じ値だが、元データに合わせる）
    r, _ = pearsonr(x, y_ms)
    print("\n[JAPAN] regression (computed in ms, shown in s)")
    print(f"  slope [s/year] = {slope_s:.4f}")
    print(f"  intercept [s]  = {intercept_s:.4f}")
    print(f"  r(Japan)       = {r:.4f}")

    plt.xlabel("release_year")
    plt.ylabel("duration [s]")
    plt.title("Japan only: duration [s] vs release_year")
    plt.grid(True)
    plt.legend(loc="upper left")

    filepath = os.path.join(save_dir, "scatter_duration_s_vs_release_year_Japan.png")
    plt.tight_layout()
    plt.savefig(filepath)
    plt.close()

    print(f"Saved scatter (Japan): {filepath}")


# =========================================================
# 2-1. 単回帰分析（Both）：release_year → duration_ms
#      （数値はこれまで通り ms 単位で計算・出力）
# =========================================================
def regression_releaseyear_duration_both(df_all):
    sub = df_all.dropna(subset=["release_year", "duration_ms"])
    x = sub["release_year"].values
    y = sub["duration_ms"].values

    result = linregress(x, y)
    n = len(x)

    print("\n===== Linear regression: duration_ms = a * release_year + b (Both) =====")
    print(f"a (s/year)   = {result.slope/1000:.3f}")
    print(f"b (s)        = {result.intercept/1000:.3f}")
    print(f"r            = {result.rvalue:.3f}")
    print(f"p-value      = {result.pvalue:.3e}")
    print(f"n            = {n}")
    print("========================================================================")
    print(
        f"結論（Both）：リリース年が 1 年上がると、曲の長さは平均して約 {result.slope/1000:.1f} s 変化する。"
    )


# =========================================================
# 2-2. 単回帰分析（Japan only）：release_year → duration_ms
#      （こちらも数値は ms 単位のものをログに出す）
# =========================================================
def regression_releaseyear_duration_japan(df_japan):
    sub = df_japan.dropna(subset=["release_year", "duration_ms"])
    x = sub["release_year"].values
    y = sub["duration_ms"].values

    result = linregress(x, y)
    n = len(x)

    print("\n===== Linear regression: duration_ms = a * release_year + b (Japan only) =====")
    print(f"a (s/year)   = {result.slope/1000:.3f}")
    print(f"b (s)        = {result.intercept/1000:.3f}")
    print(f"r            = {result.rvalue:.3f}")
    print(f"p-value      = {result.pvalue:.3e}")
    print(f"n            = {n}")
    print("==============================================================================")
    print(
        f"結論（Japan）：リリース年が 1 年上がると、日本における曲の長さは平均して約 {result.slope/1000:.1f} s 変化する。"
    )


# =========================================================
# 3. t検定：Japan vs Global（平均値の差の検定）
# =========================================================
def run_ttests(df, df_japan, df_global):
    feature_cols = [
        "duration_ms",
        "popularity",
        "artist_popularity",
        "artist_followers",
        "release_year",
    ]

    print("\n================ T-tests Japan vs Global ================")
    for col in feature_cols:
        if col not in df.columns:
            print(f"{col:18s}: column not found, skip")
            continue

        j = df_japan[col].dropna()
        g = df_global[col].dropna()

        if len(j) > 1 and len(g) > 1:
            t_stat, p_val = ttest_ind(j, g, equal_var=False)
            print(
                f"{col:18s}: "
                f"t = {t_stat:8.3f}, p = {p_val:8.3e}, "
                f"n_Japan = {len(j):3d}, n_Global = {len(g):3d}"
            )
        else:
            print(f"{col:18s}: skip (not enough data)")
    print("=========================================================\n")


# =========================================================
# 4. 箱ひげ図：Global / Japan の全数値特徴量を 1 枚に
# =========================================================
def save_boxplot_all_features(df_both, save_dir):
    sns.set(style="whitegrid")

    df_features = df_both.copy()

    exclude_cols = ["country", "track_name", "artist_name"]
    numeric_cols = [
        c
        for c in df_features.columns
        if c not in exclude_cols and np.issubdtype(df_features[c].dtype, np.number)
    ]

    print("Boxplot に使う列:", numeric_cols)

    df_long = df_features[["country"] + numeric_cols].melt(
        id_vars="country",
        var_name="feature_name",
        value_name="value_raw",
    )

    # 各特徴量ごとに標準化（平均0, 分散1）
    df_long["value"] = df_long.groupby("feature_name")["value_raw"].transform(
        lambda x: (x - x.mean()) / x.std(ddof=0)
    )

    plt.figure(figsize=(8, 4))
    ax = sns.boxplot(
        data=df_long,
        x="country",
        y="value",
        hue="feature_name",
        order=["Global", "Japan"],
        showfliers=True,
    )

    plt.title("Comparison of the top 50 Spotify weekly rankings in each country.")
    plt.xlabel("country")
    plt.ylabel("value (standardized)")

    # 凡例（タイトルは消す）
    handles, labels = ax.get_legend_handles_labels()
    plt.legend(
        handles,
        labels,
        title="",
        bbox_to_anchor=(1.02, 1),
        loc="upper left",
        borderaxespad=0.0,
    )

    plt.tight_layout()
    box_path = os.path.join(save_dir, "box_japan_global_features.png")
    plt.savefig(box_path)
    plt.close()

    print(f"Boxplot saved to: {box_path}")


# =========================================================
# メイン処理
# =========================================================
if __name__ == "__main__":
    # 散布図（Both, duration を秒で表示）
    save_scatter_release_vs_duration_both(df_both, df_japan, df_global, SAVE_DIR)

    # 散布図（Japan only, 秒＋回帰直線＋式）
    save_scatter_release_vs_duration_japan(df_japan, SAVE_DIR)

    # 単回帰（Both, ms 単位でログ出力）
    regression_releaseyear_duration_both(df_both)

    # 単回帰（Japan only, ms 単位でログ出力）
    regression_releaseyear_duration_japan(df_japan)

    # t検定
    run_ttests(df, df_japan, df_global)

    # 箱ひげ図
    save_boxplot_all_features(df_both, SAVE_DIR)

    print("Saved:", SAVE_DIR)
    
