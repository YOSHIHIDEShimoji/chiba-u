# collect_combined.py
# 各サブフォルダの combined.png を
# 親フォルダ/combined/<サブフォルダ名>.png にコピーする

import os
import shutil
import sys

def main(parent_dir):
    if not os.path.isdir(parent_dir):
        print("親ディレクトリが見つかりません:", parent_dir)
        sys.exit(1)

    # 保存先ディレクトリを作成（既にあればOK）
    combined_dir = os.path.join(parent_dir, "combined")
    os.makedirs(combined_dir, exist_ok=True)

    # サブフォルダを走査
    subdirs = sorted(
        d for d in os.listdir(parent_dir)
        if os.path.isdir(os.path.join(parent_dir, d)) and not d.startswith(".")
    )

    copied = 0
    for sub in subdirs:
        src = os.path.join(parent_dir, sub, "combined.png")
        if os.path.exists(src):
            dst = os.path.join(combined_dir, f"{sub}.png")
            shutil.copy2(src, dst)
            print(f"[OK] {sub}/combined.png → combined/{sub}.png")
            copied += 1
        else:
            print(f"[SKIP] {sub}: combined.png が見つかりません")

    print(f"\n=== コピー完了 ({copied} 個のファイル) ===")
    print(f"保存先: {combined_dir}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("使い方: python3 collect_combined.py /path/to/親ディレクトリ")
        sys.exit(1)
    main(sys.argv[1])
