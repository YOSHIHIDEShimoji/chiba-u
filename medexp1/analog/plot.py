#!/usr/bin/env python3

import sys, os, re, argparse
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib import font_manager as fm
from matplotlib import colors as mcolors

DEFAULT_COLOR_CH1 = "orange"
DEFAULT_COLOR_CH2 = "deepskyblue"
DEFAULT_COLOR_CONST = "red"

# ---------------- 日本語フォント設定（文字化け回避） ----------------
CANDIDATE_FONTS = [
    "Hiragino Sans", "Hiragino Kaku Gothic ProN",
    "Yu Gothic", "YuGothic",
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

# ---------------- 色の妥当性チェック ----------------
def validate_color_or_die(name: str, value: str):
    if value is None:
        return
    if not mcolors.is_color_like(value):
        raise ValueError(f"Invalid matplotlib color for {name}: '{value}'")

# ---------------- Tek CSV から DE 列と単位を抽出 ----------------
def read_tek_csv_de(lines):
    """Tek 系 CSV から D/E（4,5列）を time/value として抽出し、単位も返す"""
    units_time, units_value = None, None
    for ln in lines[:100]:
        if "Horizontal Units" in ln:
            m = re.search(r"Horizontal Units\s*,?\s*([A-Za-zµμ/]+)", ln)
            if m:
                units_time = m.group(1)
        if "Vertical Units" in ln:
            m = re.search(r"Vertical Units\s*,?\s*([A-Za-zµμ/]+)", ln)
            if m:
                units_value = m.group(1)

    start_idx = None
    for i, ln in enumerate(lines):
        parts = ln.strip().split(",")
        if len(parts) >= 5:
            try:
                float(parts[3])
                float(parts[4])
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
_LATEX_WORD = re.compile(r'_(?!\{)([A-Za-z0-9+\-]+)')

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

# ---------------- ラベルから Vi / Vo を推定（意味ベース色指定用） ----------------
def infer_vi_vo_role(label: str):
    """
    label から "vi" / "vo" を推定して返す。
    例: V_i, Vi, V_{i1}, V_{i+}, Vin -> vi
        V_o, Vo, Vout              -> vo
    推定できなければ None
    """
    if label is None:
        return None
    s = label.strip()
    if len(s) == 0:
        return None
    s = s.replace("$", "")
    s = re.sub(r"[{}\s\\]", "", s)
    s = s.lower()

    if "vi" in s or "v_i" in s or "vin" in s:
        return "vi"
    if "vo" in s or "v_o" in s or "vout" in s:
        return "vo"
    return None

def first_effective_label(labels):
    if labels is None:
        return None
    lst = labels if isinstance(labels, list) else [labels]
    if not lst:
        return None
    head = str(lst[0]).strip()
    if head.lower() == "none":
        return None
    return head

# ---------------- セッション判定と CSV 探索 ----------------
def find_ch_paths(session_dir):
    """セッションフォルダ内の CH1/CH2 CSV（大文字小文字）を探しパスを返す"""
    ch1 = os.path.join(session_dir, "F0000CH1.CSV")
    if not os.path.exists(ch1):
        ch1 = os.path.join(session_dir, "F0000CH1.csv")
    ch2 = os.path.join(session_dir, "F0000CH2.CSV")
    if not os.path.exists(ch2):
        ch2 = os.path.join(session_dir, "F0000CH2.csv")
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

def resolve_add_file(session_dir, add_file):
    if add_file is None:
        return None
    if os.path.isabs(add_file):
        return add_file
    return os.path.join(session_dir, add_file)

# ---------------- 描画と保存（PNG / 任意で XLSX） ----------------
def plot_one_session(
    session_dir,
    ch1_labels,
    ch2_labels,
    const_label,
    const_value,
    add_label=None,
    add_file=None,
    save_xlsx=False,
    color_ch1=DEFAULT_COLOR_CH1,
    color_ch2=DEFAULT_COLOR_CH2,
    color_add=DEFAULT_COLOR_CONST
):
    ch1, ch2 = find_ch_paths(session_dir)
    if ch1 is None or ch2 is None:
        raise FileNotFoundError("F0000CH1.CSV もしくは F0000CH2.CSV が見つかりません")

    df1, ut1, uv1 = load_channel_csv(ch1)
    df2, ut2, uv2 = load_channel_csv(ch2)

    df3 = None
    ut3 = None
    uv3 = None

    if (add_label is not None) or (add_file is not None):
        if (add_label is None) or (add_file is None):
            print("[WARN] --add-label と --add-file は両方指定してください。追加CSVの描画はスキップします。", file=sys.stderr)
        else:
            add_path = resolve_add_file(session_dir, add_file)
            if not os.path.exists(add_path):
                print(f"[WARN] 追加CSVが見つかりません: {add_path}（スキップ）", file=sys.stderr)
            else:
                try:
                    df3, ut3, uv3 = load_channel_csv(add_path)
                except Exception as e:
                    print(f"[WARN] 追加CSVの読み込みに失敗しました: {add_path}: {e}（スキップ）", file=sys.stderr)
                    df3 = None

    unit_time = ut1 or ut2 or ut3 or "s"
    unit_val = uv1 or uv2 or uv3 or "V"

    fig, ax = plt.subplots(figsize=(9, 5))

    if ch1_labels is not None:
        ch1_list = ch1_labels if isinstance(ch1_labels, list) else [ch1_labels]
        if len(ch1_list) > 0 and str(ch1_list[0]).lower() != "none":
            line1, = ax.plot(df1["time"], df1["value"], color=color_ch1, label=latexify(ch1_list[0]))
            for extra in ch1_list[1:]:
                ax.plot([], [], color=line1.get_color(), label=latexify(extra))

    if ch2_labels is not None:
        ch2_list = ch2_labels if isinstance(ch2_labels, list) else [ch2_labels]
        if len(ch2_list) > 0 and str(ch2_list[0]).lower() != "none":
            line2, = ax.plot(df2["time"], df2["value"], color=color_ch2, label=latexify(ch2_list[0]))
            for extra in ch2_list[1:]:
                ax.plot([], [], color=line2.get_color(), label=latexify(extra))

    if (df3 is not None) and (add_label is not None):
        ax.plot(df3["time"], df3["value"], color=color_add, linewidth=1.5, label=latexify(add_label))

    if (const_label is not None) or (const_value is not None):
        if (const_label is None) or (const_value is None):
            print("[WARN] --const-label と --const-value は両方指定してください。定数線はスキップします。", file=sys.stderr)
        else:
            ax.axhline(
                float(const_value),
                color=color_add,
                linewidth=1.5,
                label=f"{latexify(const_label)} = {float(const_value):g}"
            )

    ax.set_xlabel(f"時間[{unit_time}]")
    ax.set_ylabel(f"電圧[{unit_val}]")
    ax.legend(loc="lower right")
    fig.tight_layout()

    out_png = os.path.join(session_dir, "combined.png")
    fig.savefig(out_png, dpi=150)
    plt.close(fig)

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
                if df3 is not None:
                    df3.to_excel(w, sheet_name="ADD_DE", index=False)
        except Exception as e:
            print(f"[WARN] Excel 出力に失敗しました: {e}", file=sys.stderr)

    return out_png

# ---------------- メイン（単発／一括を自動判定） ----------------
def main():
    epilog = r"""
使用例:

  1) 既定設定（CH1=V_i, CH2=V_o）で 1 セッションを描画
     python3 plot.py /path/to/session

  2) CH1 に複数の凡例を付ける（波形は1本，凡例のみ複数）
     python3 plot.py /path/to/session --ch1 V_{i1} V_{i2} --ch2 V_o

  3) CH1 を非表示にする
     python3 plot.py /path/to/session --ch1 none --ch2 V_o

  4) 定数値を水平線として描画する（例: V_{i-} = -3.0）
     python3 plot.py /path/to/session --const-label V_{i-} --const-value -3.0

  5) 定数をオシロスコープで取得した CSV で重ね描画する（例: F0000CH3.CSV）
     python3 plot.py /path/to/session --add-label V_{i-} --add-file F0000CH3.CSV

  6) Vi / Vo の意味ベースで色を指定（判定できないラベルは CH色へフォールバック）
     python3 plot.py /path/to/session --ch1 V_i --ch2 V_o --color-vi orange --color-vo deepskyblue --color-add red

  7) CHベースで色指定（互換用）
     python3 plot.py /path/to/session --ch1 V_o --ch2 V_{fo} --color-ch1 deepskyblue --color-ch2 red

  8) 親フォルダ配下を一括処理し，Excel（merged.xlsx）も保存
     python3 plot.py /path/to/parent --xlsx
"""
    ap = argparse.ArgumentParser(
        prog="plot.py",
        description=(
            "Tektronix オシロスコープの CSV（CH1/CH2）を読み込み，"
            "柔軟な凡例ラベルと定数（水平線または追加CSV）を重ねて描画します。\n"
            "指定パスがセッションフォルダ（CH1/CH2 CSV を含む）の場合は単発処理，\n"
            "親フォルダの場合は配下のセッションフォルダを自動検出して一括処理します。\n\n"
            "定数は，数値指定（--const-label / --const-value）か，\n"
            "CSV 指定（--add-label / --add-file）のどちらか一方を使用してください（同時使用は想定していません）。\n"
        ),
        epilog=epilog,
        formatter_class=argparse.RawDescriptionHelpFormatter
    )

    ap.add_argument("target_path", help="処理対象のパス（セッションフォルダ または 親フォルダ）")

    ap.add_argument("--ch1", nargs="+", default=["V_i"],
                    help="CH1 の凡例ラベル（複数指定可）。'none' を先頭に指定すると CH1 を非表示にする")
    ap.add_argument("--ch2", nargs="+", default=["V_o"],
                    help="CH2 の凡例ラベル（複数指定可）。'none' を先頭に指定すると CH2 を非表示にする")

    ap.add_argument("--const-label",
                    help="定数を表すラベル（水平線として描画する場合）。--const-value とセットで指定")
    ap.add_argument("--const-value", type=float,
                    help="定数の値（水平線として描画する場合）。--const-label とセットで指定")

    ap.add_argument("--add-label",
                    help="追加CSV（例: CH3相当）を重ね描画する際の凡例ラベル。--add-file とセットで指定")
    ap.add_argument("--add-file",
                    help="追加CSVファイル名またはパス（Tektronix CSV 形式）。--add-label とセットで指定")

    ap.add_argument("--color-vi", default=None,
                    help="Vi の色（意味ベース指定）。ラベルから Vi/Vo を推定して割り当てる（推定不能は CH色へフォールバック）")
    ap.add_argument("--color-vo", default=None,
                    help="Vo の色（意味ベース指定）。ラベルから Vi/Vo を推定して割り当てる（推定不能は CH色へフォールバック）")
    ap.add_argument("--color-add", default=None,
                    help="定数の色（水平線 または 追加CSV の描画色）")

    ap.add_argument("--color-ch1", default=None,
                    help="CH1 の線色（互換用）。意味ベースで色が決まる場合はそちらが優先される")
    ap.add_argument("--color-ch2", default=None,
                    help="CH2 の線色（互換用）。意味ベースで色が決まる場合はそちらが優先される")

    ap.add_argument("--xlsx", action="store_true",
                    help="各セッションフォルダに merged.xlsx を保存する")

    args = ap.parse_args()
    target = os.path.abspath(args.target_path)

    if not os.path.exists(target):
        print(f"指定パスが見つかりません: {target}", file=sys.stderr)
        sys.exit(1)

    meaning_mode = (args.color_vi is not None) or (args.color_vo is not None)

    vi_color = args.color_vi or DEFAULT_COLOR_CH1
    vo_color = args.color_vo or DEFAULT_COLOR_CH2
    add_color = args.color_add or DEFAULT_COLOR_CONST

    ch1_fallback = args.color_ch1 or DEFAULT_COLOR_CH1
    ch2_fallback = args.color_ch2 or DEFAULT_COLOR_CH2

    try:
        validate_color_or_die("color_vi", vi_color)
        validate_color_or_die("color_vo", vo_color)
        validate_color_or_die("color_add", add_color)
        validate_color_or_die("color_ch1", ch1_fallback)
        validate_color_or_die("color_ch2", ch2_fallback)
    except Exception as e:
        print(f"[ERROR] {e}", file=sys.stderr)
        sys.exit(1)

    def colors_for_labels(ch1_labels, ch2_labels):
        c1 = ch1_fallback
        c2 = ch2_fallback

        if meaning_mode:
            l1 = first_effective_label(ch1_labels)
            l2 = first_effective_label(ch2_labels)
            r1 = infer_vi_vo_role(l1) if l1 is not None else None
            r2 = infer_vi_vo_role(l2) if l2 is not None else None

            if l1 is not None:
                if r1 == "vi":
                    c1 = vi_color
                elif r1 == "vo":
                    c1 = vo_color

            if l2 is not None:
                if r2 == "vi":
                    c2 = vi_color
                elif r2 == "vo":
                    c2 = vo_color

        return c1, c2

    made = 0

    if os.path.isdir(target) and is_session_dir(target):
        try:
            c1, c2 = colors_for_labels(args.ch1, args.ch2)
            out = plot_one_session(
                target,
                args.ch1,
                args.ch2,
                args.const_label,
                args.const_value,
                add_label=args.add_label,
                add_file=args.add_file,
                save_xlsx=args.xlsx,
                color_ch1=c1,
                color_ch2=c2,
                color_add=add_color
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
                c1, c2 = colors_for_labels(args.ch1, args.ch2)
                out = plot_one_session(
                    sd,
                    args.ch1,
                    args.ch2,
                    args.const_label,
                    args.const_value,
                    add_label=args.add_label,
                    add_file=args.add_file,
                    save_xlsx=args.xlsx,
                    color_ch1=c1,
                    color_ch2=c2,
                    color_add=add_color
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
