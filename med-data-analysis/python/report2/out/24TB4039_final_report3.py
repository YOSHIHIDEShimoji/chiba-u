import numpy as np
from PIL import Image
import pydicom
import sys


def dcm_window_processing(in_file_name, out_file_name):

    dcm_data = pydicom.dcmread(in_file_name)

    # 画素データの取得と画素値の変換 (CTの場合はCT値[H.U.]になる)
    img = dcm_data.pixel_array
    img = img * dcm_data.RescaleSlope + dcm_data.RescaleIntercept

    # ウィンドウ処理の範囲[a, b]の決定
    if dcm_data.Modality == "CT":
        # CT: Window Level -600 [H.U.], Window Width 1500 [H.U.]
        window_level = -600
        window_width = 1500
        a = window_level - window_width // 2
        b = window_level + window_width // 2
    else:
        # CT以外: 画素値の[最小値, 最大値]
        a = img.min()
        b = img.max()

    print(f"{in_file_name}: Modality={dcm_data.Modality}, window=[{a}, {b}]")

    # [a, b]を[0, 255]に線形変換し、範囲外は0または255に丸める
    processed_img = 255.0 * (img.astype(np.float64) - a) / (b - a)
    processed_img = processed_img.clip(0, 255)

    # 8bitグレースケール画像として保存
    processed_img = processed_img.astype(np.uint8)
    pil_image = Image.fromarray(processed_img)
    pil_image.save(out_file_name)

    return processed_img


def main():

    if len(sys.argv) != 3:
        print(f"Usage: python {sys.argv[0]} <input DICOM file> <output PNG file>")
        sys.exit(1)

    # コマンドライン引数から入出力ファイル名を取得
    in_file_name = sys.argv[1]
    out_file_name = sys.argv[2]

    dcm_window_processing(in_file_name, out_file_name)


if __name__ == "__main__":
    main()
