#!/usr/bin/env python3
import math
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
from matplotlib import font_manager as fm

def set_japanese_font():
    candidates = [
        "Hiragino Sans", "Hiragino Kaku Gothic ProN",
        "Yu Gothic", "YuGothic",
        "Noto Sans CJK JP", "Noto Sans JP",
        "IPAexGothic", "IPAGothic",
    ]
    for name in candidates:
        try:
            _ = fm.findfont(name, fallback_to_default=False)
            plt.rcParams["font.family"] = name
            break
        except Exception:
            continue
    plt.rcParams["axes.unicode_minus"] = False

BASIC_COLORS = [
    "red", "blue", "green", "orange", "purple",
    "yellow", "black", "white", "gray",
]

COMMON_EXTRA_COLORS = [
    "deepskyblue", "dodgerblue", "steelblue", "navy", "teal", "cyan",
    "darkred", "firebrick", "crimson", "maroon",
    "lime", "forestgreen", "darkgreen",
    "gold", "goldenrod", "khaki",
    "pink", "hotpink", "deeppink",
    "brown", "saddlebrown", "chocolate",
]

TAB_COLORS = [
    "tab:blue", "tab:orange", "tab:green", "tab:red", "tab:purple",
    "tab:brown", "tab:pink", "tab:gray", "tab:olive", "tab:cyan",
]

def draw_block(ax, colors, y_top, ncols, cell_w, cell_h, pad, name_fs):
    rows = math.ceil(len(colors) / ncols)

    for i, name in enumerate(colors):
        r = i // ncols
        c = i % ncols
        x = c * cell_w
        y = y_top - r * cell_h

        rect = Rectangle(
            (x + pad, y - cell_h + pad),
            cell_w - 2 * pad,
            cell_h - 2 * pad,
            facecolor=name,
            edgecolor="black",
            linewidth=0.6,
        )
        ax.add_patch(rect)

        ax.text(
            x + cell_w / 2,
            y - cell_h / 2 - 0.3,
            name,
            ha="center",
            va="top",
            fontsize=name_fs,
        )

    return y_top - rows * cell_h

def main():
    set_japanese_font()

    blocks = [
        BASIC_COLORS,
        COMMON_EXTRA_COLORS,
        TAB_COLORS,
    ]

    ncols = 8
    cell_w = 1.25
    cell_h = 0.75
    pad = 0.10
    name_fs = 8

    total_rows = sum(math.ceil(len(colors) / ncols) for colors in blocks)
    height = max(4.5, total_rows * cell_h + 1.5)
    width = ncols * cell_w + 0.6

    fig, ax = plt.subplots(figsize=(width, height))
    ax.set_xlim(0, ncols * cell_w)
    ax.set_ylim(0, height)
    ax.axis("off")

    y = height - 0.5
    for colors in blocks:
        y = draw_block(
            ax,
            colors,
            y_top=y,
            ncols=ncols,
            cell_w=cell_w,
            cell_h=cell_h,
            pad=pad,
            name_fs=name_fs,
        )
        y -= cell_h * 0.6  # ブロック間の余白

    fig.suptitle("Matplotlib color name preview", fontsize=14, y=0.99)
    fig.tight_layout(pad=0.6)
    plt.show()

if __name__ == "__main__":
    main()