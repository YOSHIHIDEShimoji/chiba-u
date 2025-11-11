#!/usr/bin/env python3
# plot.py
# Tektronix CSV (CH1/CH2) を読み、柔軟ラベル＋任意の定数水平線を描画
# 各セッションフォルダに combined.png（必要に応じて merged.xlsx）を保存

import sys, os, re, argparse
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib import font_manager as fm

# ---------------- 日本語フォント設定（文字化け回避） ----------------
CANDIDATE_FONTS = [
    "Hiragino Sans", "Hiragino Kaku Gothic ProN",  # macOS
    "Yu Gothic", "YuGothic",                      # Windows
    "Noto Sans CJK JP", "Noto Sans JP",
    "IPAexGothic", "IPAGothic"
]
for name in CANDIDATE_FONTS:
    try:
        _ = fm.findfont(name, fallback_to_default=False)
        plt.rcParams["font.family"] = name
        break
    except Exception:
        continue
plt.rcParams["axes.unicode_minus"] = False

# ---------------- Tek CSV から DE 列と単位を抽出 ----------------
def read_tek_csv_de(lines):
    """Tek 系 CSV から D/E（4,5列）を time/value として抽出し、単位も返す"""
    units_time, units_value = None, None
    for ln in lines[:100]:
        if "Horizontal Units" in ln:
            m = re.search(r"Horizontal Units\s*,?\s*([A-Za-zµμ/]+)", ln)
            if m: units_time = m.group(1)
        if "Vertical Units" in ln:
            m = re.search(r"Vertical Units\s*,?\s*([A-Za-zµμ/]+)", ln)
            if m: units_value = m.group(1)

    # データ開始行（D/E が数値の最初の行）
    start_idx = None
    for i, ln in enumerate(lines):
        parts = ln.strip().split(",")
        if len(parts) >= 5:
            try:
                float(parts[3]); float(parts[4])
                start_idx = i
                break
            except Exception:
                pass
    if start_idx is None:
        raise ValueError("データ開始行を特定できません。CSV を確認してください")

    from io import StringIO
    data_s = "\n".join(lines[start_idx:])
    df = pd.read_csv(StringIO(data_s), header=None)
    if df.shape[1] < 5:
        raise ValueError("5列未満のデータです（D/E 列不足）")

    df_de = df.iloc[:, 3:5].copy()
    df_de.columns = ["time", "value"]
    return df_de, units_time, units_value

def load_channel_csv(path_csv):
    with open(path_csv, "r", encoding="utf-8", errors="ignore") as f:
        lines = f.read().splitlines()
    return read_tek_csv_de(lines)

# ---------------- ラベル（文字列）→ LaTeX 風 mathtext へ整形 ----------------
_LATEX_WORD = re.compile(r'_(?!\{)([A-Za-z0-9+\-]+)')  # 例: V_i → V_{i}, V_10 → V_{10}, V_i- → V_{i-}

def latexify(label: str) -> str:
    """
    入力: "V_i", "V_1", "V_{i-}", "Vin", "$V_{o}$" など
    出力: "$V_{i}$" のように mathtext として解釈できる形に（既に $...$ ならそのまま）
    """
    if label is None:
        return None
    s = label.strip()
    if len(s) == 0:
        return s
    if s.startswith("$") and s.endswith("$"):
        return s
    s = _LATEX_WORD.sub(r'_{\1}', s)
    return f"${s}$"

# ---------------- セッション判定と CSV 探索 ----------------
def find_ch_paths(session_dir):
    """セッションフォルダ内の CH1/CH2 CSV（大文字小文字）を探しパスを返す"""
    ch1 = os.path.join(session_dir, "F0000CH1.CSV")
    if not os.path.exists(ch1): ch1 = os.path.join(session_dir, "F0000CH1.csv")
    ch2 = os.path.join(session_dir, "F0000CH2.CSV")
    if not os.path.exists(ch2): ch2 = os.path.join(session_dir, "F0000CH2.csv")
    if os.path.exists(ch1) and os.path.exists(ch2):
        return ch1, ch2
    return None, None

def is_session_dir(path):
    ch1, ch2 = find_ch_paths(path)
    return (ch1 is not None) and (ch2 is not None)

def list_session_dirs(parent):
    """親フォルダ配下のセッション候補（サブフォルダ）を列挙"""
    items = []
    for d in sorted(os.listdir(parent)):
        p = os.path.join(parent, d)
        if os.path.isdir(p) and not d.startswith("."):
            items.append(p)
    return items

# ---------------- 描画と保存（PNG / 任意で XLSX） ----------------
def plot_one_session(session_dir, ch1_labels, ch2_labels, const_label, const_value, save_xlsx=False):
    ch1, ch2 = find_ch_paths(session_dir)
    if ch1 is None or ch2 is None:
        raise FileNotFoundError("F0000CH1.CSV もしくは F0000CH2.CSV が見つかりません")

    df1, ut1, uv1 = load_channel_csv(ch1)  # CH1
    df2, ut2, uv2 = load_channel_csv(ch2)  # CH2

    unit_time = ut1 or ut2 or "s"
    unit_val  = uv1 or uv2 or "V"

    fig, ax = plt.subplots(figsize=(9, 5))

    # --- CH1（複数ラベル対応・none対応） ---
    if ch1_labels is not None:
        ch1_list = ch1_labels if isinstance(ch1_labels, list) else [ch1_labels]
        if len(ch1_list) > 0 and str(ch1_list[0]).lower() != "none":
            line1, = ax.plot(df1["time"], df1["value"], color="orange", label=latexify(ch1_list[0]))
            for extra in ch1_list[1:]:
                ax.plot([], [], color=line1.get_color(), label=latexify(extra))

    # --- CH2（複数ラベル対応・none対応） ---
    if ch2_labels is not None:
        ch2_list = ch2_labels if isinstance(ch2_labels, list) else [ch2_labels]
        if len(ch2_list) > 0 and str(ch2_list[0]).lower() != "none":
            line2, = ax.plot(df2["time"], df2["value"], color="deepskyblue", label=latexify(ch2_list[0]))
            for extra in ch2_list[1:]:
                ax.plot([], [], color=line2.get_color(), label=latexify(extra))

    # 定数線（両方指定された場合のみ）
    if (const_label is not None) or (const_value is not None):
        if (const_label is None) or (const_value is None):
            print("[WARN] --const-label と --const-value は両方指定してください。定数線はスキップします。", file=sys.stderr)
        else:
            ax.axhline(float(const_value), color="red", linewidth=1.5,
                   label=f"{latexify(const_label)} = {float(const_value):g}")


    ax.set_xlabel(f"時間[{unit_time}]")
    ax.set_ylabel(f"電圧[{unit_val}]")
    ax.legend(loc="lower right")
    fig.tight_layout()

    out_png = os.path.join(session_dir, "combined.png")
    fig.savefig(out_png, dpi=150)
    plt.close(fig)

    # 任意: Excel 保存（CH1/CH2 の AB/DE 形式が必要な場合）
    if save_xlsx:
        try:
            def read_ab(path_csv):
                with open(path_csv, "r", encoding="utf-8", errors="ignore") as f:
                    lines = f.read().splitlines()
                from io import StringIO
                df_all = pd.read_csv(StringIO("\n".join(lines)), header=None)
                if df_all.shape[1] >= 2:
                    df_ab = df_all.iloc[:, 0:2].copy()
                    df_ab.columns = ["A", "B"]
                    return df_ab
                return None

            df1_ab = read_ab(ch1)
            df2_ab = read_ab(ch2)

            out_xlsx = os.path.join(session_dir, "merged.xlsx")
            with pd.ExcelWriter(out_xlsx, engine="openpyxl") as w:
                if df1_ab is not None:
                    df1_ab.to_excel(w, sheet_name="CH1_AB", index=False)
                df1.to_excel(w, sheet_name="CH1_DE", index=False)
                if df2_ab is not None:
                    df2_ab.to_excel(w, sheet_name="CH2_AB", index=False)
                df2.to_excel(w, sheet_name="CH2_DE", index=False)
        except Exception as e:
            print(f"[WARN] Excel 出力に失敗しました: {e}", file=sys.stderr)

    return out_png

# ---------------- メイン（単発／一括を自動判定） ----------------
def main():
    epilog = r"""
使用例:
  1) 既定（CH1=V_i, CH2=V_o）で 1 セッションを描画
     python3 plot.py /path/to/session

  2) CH1 を V_{i1}, V_{i2} として凡例を2つ、CH2 を V_o
     python3 plot.py /path/to/session --ch1 V_{i1} V_{i2} --ch2 V_o

  3) CH2 を V_o, V_{out} として凡例を2つ（CH1は非表示）
     python3 plot.py /path/to/session --ch1 none --ch2 V_o V_{out}

  4) 定数 V_{i-} = -3.0 を追加
     python3 plot.py /path/to/session --const-label V_{i-} --const-value -3.0

  5) 親フォルダ配下を一括処理し、Excel も保存
     python3 plot.py /path/to/parent --xlsx
"""
    ap = argparse.ArgumentParser(
        prog="plot.py",
        description="Tektronix CSV（CH1/CH2）を読み、柔軟な凡例ラベルと任意の定数水平線を加えてプロットします。\n"
                    "対象がセッション直下（CH1/CH2 がある）なら単発処理、親フォルダなら配下サブフォルダを一括処理します。",
        epilog=epilog,
        formatter_class=argparse.RawDescriptionHelpFormatter
    )
    ap.add_argument("target_path", help="セッションフォルダ もしくは 親フォルダ のパス")
    ap.add_argument("--ch1", nargs="+", default=["V_i"],
                    help="CH1 の凡例ラベル（複数指定可: --ch1 V_{i1} V_{i2}）。先頭に 'none' で非表示")
    ap.add_argument("--ch2", nargs="+", default=["V_o"],
                    help="CH2 の凡例ラベル（複数指定可: --ch2 V_o V_{out}）。先頭に 'none' で非表示")
    ap.add_argument("--const-label", help="定数線の凡例ラベル（例: V_{i-}, V_ref）。--const-value とセットで指定")
    ap.add_argument("--const-value", type=float, help="定数線の値（例: -3.0）。--const-label とセットで指定")
    ap.add_argument("--xlsx", action="store_true", help="merged.xlsx を各セッションに保存（任意）")

    args = ap.parse_args()
    target = os.path.abspath(args.target_path)

    if not os.path.exists(target):
        print(f"指定パスが見つかりません: {target}", file=sys.stderr)
        sys.exit(1)

    made = 0
    if os.path.isdir(target) and is_session_dir(target):
        try:
            out = plot_one_session(
                target, args.ch1, args.ch2, args.const_label, args.const_value, save_xlsx=args.xlsx
            )
            print(f"[OK] {target} -> {os.path.basename(out)}")
            made += 1
        except Exception as e:
            print(f"[SKIP] {target}: {e}", file=sys.stderr)
    elif os.path.isdir(target):
        sessions = list_session_dirs(target)
        if not sessions:
            print("サブフォルダが見つかりません", file=sys.stderr)
            sys.exit(1)
        for sd in sessions:
            if not is_session_dir(sd):
                continue
            try:
                out = plot_one_session(
                    sd, args.ch1, args.ch2, args.const_label, args.const_value, save_xlsx=args.xlsx
                )
                print(f"[OK] {sd} -> {os.path.basename(out)}")
                made += 1
            except Exception as e:
                print(f"[SKIP] {sd}: {e}", file=sys.stderr)
        if made == 0:
            print("処理できたセッションがありませんでした", file=sys.stderr)
    else:
        print("指定パスはフォルダではありません", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()
