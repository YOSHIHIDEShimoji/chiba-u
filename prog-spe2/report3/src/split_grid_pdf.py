import argparse
from pathlib import Path

import fitz  # PyMuPDF
import numpy as np
from PIL import Image


def render_pdf_page(pdf_path: Path, page_index: int, dpi: int) -> Image.Image:
    doc = fitz.open(str(pdf_path))
    page = doc.load_page(page_index)
    zoom = dpi / 72.0
    mat = fitz.Matrix(zoom, zoom)
    pix = page.get_pixmap(matrix=mat, alpha=False)
    img = Image.frombytes("RGB", [pix.width, pix.height], pix.samples)
    doc.close()
    return img


def find_content_bbox(img: Image.Image, thr: int) -> tuple[int, int, int, int]:
    gray = np.array(img.convert("L"))
    mask = gray < thr
    if not mask.any():
        return (0, 0, img.width, img.height)
    ys, xs = np.where(mask)
    x0, x1 = int(xs.min()), int(xs.max())
    y0, y1 = int(ys.min()), int(ys.max())
    return (x0, y0, x1 + 1, y1 + 1)


def crop_with_margin(bbox, w, h, margin_px: int):
    x0, y0, x1, y1 = bbox
    x0 = max(0, x0 - margin_px)
    y0 = max(0, y0 - margin_px)
    x1 = min(w, x1 + margin_px)
    y1 = min(h, y1 + margin_px)
    return (x0, y0, x1, y1)


def preprocess_cell(cell: Image.Image, out_size: int | None, invert: bool) -> Image.Image:
    g = cell.convert("L")
    arr = np.array(g).astype(np.uint8)
    if invert:
        arr = 255 - arr
    out = Image.fromarray(arr, mode="L")
    if out_size is not None:
        out = out.resize((out_size, out_size), Image.BILINEAR)
    return out


def split_and_save(
    page_img: Image.Image,
    out_dir: Path,
    page_no: int,
    grid_n: int,
    content_thr: int,
    bbox_margin_ratio: float,
    inner_pad_ratio: float,
    out_size: int | None,
    invert: bool,
):
    bbox = find_content_bbox(page_img, thr=content_thr)
    margin_px = int(min(page_img.width, page_img.height) * bbox_margin_ratio)
    bbox = crop_with_margin(bbox, page_img.width, page_img.height, margin_px)

    x0, y0, x1, y1 = bbox
    crop = page_img.crop((x0, y0, x1, y1))

    cw = crop.width / grid_n
    ch = crop.height / grid_n
    pad_x = int(cw * inner_pad_ratio)
    pad_y = int(ch * inner_pad_ratio)

    for r in range(grid_n):
        for c in range(grid_n):
            left = int(round(c * cw)) + pad_x
            top = int(round(r * ch)) + pad_y
            right = int(round((c + 1) * cw)) - pad_x
            bottom = int(round((r + 1) * ch)) - pad_y

            left = max(0, min(left, crop.width - 1))
            top = max(0, min(top, crop.height - 1))
            right = max(left + 1, min(right, crop.width))
            bottom = max(top + 1, min(bottom, crop.height))

            cell = crop.crop((left, top, right, bottom))
            cell = preprocess_cell(cell, out_size=out_size, invert=invert)

            digit = r
            fname = f"{digit}_p{page_no:02d}_r{r:02d}_c{c:02d}.png"
            cell.save(out_dir / fname)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("pdf", type=str)
    ap.add_argument("--out", type=str, default="out_images")
    ap.add_argument("--grid", type=int, default=10)
    ap.add_argument("--dpi", type=int, default=200)
    ap.add_argument("--out_size", type=int, default=0)  # 0ならリサイズしない
    ap.add_argument("--invert", action="store_true")
    ap.add_argument("--content_thr", type=int, default=240)
    ap.add_argument("--bbox_margin_ratio", type=float, default=0.01)
    ap.add_argument("--inner_pad_ratio", type=float, default=0.08)
    args = ap.parse_args()

    pdf_path = Path(args.pdf)
    out_dir = Path(args.out)
    out_dir.mkdir(parents=True, exist_ok=True)

    doc = fitz.open(str(pdf_path))
    num_pages = doc.page_count
    doc.close()

    out_size = None if args.out_size == 0 else args.out_size

    for p in range(num_pages):
        page_img = render_pdf_page(pdf_path, p, dpi=args.dpi)
        split_and_save(
            page_img=page_img,
            out_dir=out_dir,
            page_no=p + 1,
            grid_n=args.grid,
            content_thr=args.content_thr,
            bbox_margin_ratio=args.bbox_margin_ratio,
            inner_pad_ratio=args.inner_pad_ratio,
            out_size=out_size,
            invert=args.invert,
        )

    print(f"saved_dir={out_dir}")
    print(f"pages={num_pages}, grid={args.grid}x{args.grid}, total_images={num_pages * args.grid * args.grid}")


if __name__ == "__main__":
    main()
