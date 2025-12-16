import cv2
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path

INPUT_DIR = Path('InputPictures')
OUTPUT_DIR = Path('OutputPictures')
SHOW_WINDOWS = False  # True にすると元の OpenCV ウィンドウ表示を行う
SHOW_PLOTS = False  # True にすると matplotlib の表示を行う
R = 1.2  # 明るさスケーリング率
Gm = 2.2  # ガンマ値


def process_image(img_path, show_windows=SHOW_WINDOWS, show_plots=SHOW_PLOTS):
    print(f'Processing {img_path.name}')
    OrgImg = cv2.imread(str(img_path))
    if OrgImg is None:
        print(f'  Skipped: failed to read {img_path}')
        return

    Img = cv2.cvtColor(OrgImg, cv2.COLOR_BGR2RGB)
    HSVImg = cv2.cvtColor(OrgImg, cv2.COLOR_BGR2HSV)

    if show_plots:
        plt.figure(figsize=(8, 8))
        plt.subplot(2, 2, 1)
        plt.imshow(Img)
        plt.subplot(2, 2, 2)
        plt.imshow(HSVImg[:, :, 0], cmap='hsv')
        plt.subplot(2, 2, 3)
        plt.imshow(HSVImg[:, :, 1], cmap='gray')
        plt.subplot(2, 2, 4)
        plt.imshow(HSVImg[:, :, 2], cmap='gray')
        plt.show()

    TmpImg = HSVImg[:, :, 0].copy()
    H, W, C = HSVImg.shape

    for j in range(H):
        for i in range(W):
            Tmp = 255 * TmpImg[j, i] / 180.0
            TmpImg[j, i] = np.clip(Tmp, 0, 255)

    cmHimg = cv2.applyColorMap(TmpImg, cv2.COLORMAP_HSV)

    if show_windows:
        cv2.imshow('Original', OrgImg)
        cv2.imshow('H', cmHimg)
        cv2.imshow('S', HSVImg[:, :, 1])
        cv2.imshow('V', HSVImg[:, :, 2])
        cv2.waitKey(0)
        cv2.destroyAllWindows()

    HSVImg2 = HSVImg.copy()
    for j in range(H):
        for i in range(W):
            HSVImg2[j, i, 2] = np.clip(R * HSVImg2[j, i, 2], 0, 255)

    Img2 = cv2.cvtColor(HSVImg2, cv2.COLOR_HSV2RGB)

    if show_plots:
        plt.figure(figsize=(8, 4))
        plt.subplot(1, 2, 1)
        plt.imshow(Img)
        plt.subplot(1, 2, 2)
        plt.imshow(Img2)
        plt.show()

    HSVImg3 = HSVImg.copy()
    for j in range(H):
        for i in range(W):
            Tmp = HSVImg3[j, i, 2] / 255.0
            HSVImg3[j, i, 2] = np.clip(np.power(Tmp, 1 / Gm) * 255, 0, 255)
    Img3 = cv2.cvtColor(HSVImg3, cv2.COLOR_HSV2RGB)

    if show_plots:
        plt.figure(figsize=(8, 4))
        plt.subplot(1, 2, 1)
        plt.imshow(Img)
        plt.subplot(1, 2, 2)
        plt.imshow(Img3)
        plt.show()

    save_dir = OUTPUT_DIR / img_path.stem
    save_dir.mkdir(parents=True, exist_ok=True)
    cv2.imwrite(str(save_dir / 'original.png'), OrgImg)
    cv2.imwrite(str(save_dir / 'h_colormap.png'), cmHimg)
    cv2.imwrite(str(save_dir / 's_channel.png'), HSVImg[:, :, 1])
    cv2.imwrite(str(save_dir / 'v_channel.png'), HSVImg[:, :, 2])
    cv2.imwrite(str(save_dir / 'value_scaled.png'), cv2.cvtColor(Img2, cv2.COLOR_RGB2BGR))
    cv2.imwrite(str(save_dir / 'gamma_corrected.png'), cv2.cvtColor(Img3, cv2.COLOR_RGB2BGR))
    print(f'  Saved outputs to {save_dir}')


def main():
    if not INPUT_DIR.exists():
        print(f'Input directory not found: {INPUT_DIR.resolve()}')
        return

    OUTPUT_DIR.mkdir(exist_ok=True)
    supported = {
        '.bmp', '.dib', '.jpg', '.jpeg', '.png', '.tif', '.tiff', '.webp', '.gif'
    }
    image_paths = sorted(
        [p for p in INPUT_DIR.iterdir() if p.suffix.lower() in supported and p.is_file()]
    )

    if not image_paths:
        print(f'No supported images found in {INPUT_DIR}')
        return

    for path in image_paths:
        process_image(path)


if __name__ == '__main__':
    main()
