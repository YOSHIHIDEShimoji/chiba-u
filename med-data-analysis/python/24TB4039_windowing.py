import numpy as np
import pydicom
from PIL import Image

dcm_data = pydicom.dcmread("sample_ct.dcm")

# オンラインテキストに貼り付けるDICOMタグ
print(dcm_data[0x0010, 0x0020])  # PatientID
print(dcm_data[0x0018, 0x0060])  # KVP

# 画素データの取得とCT値への変換
img = dcm_data.pixel_array
img = img * dcm_data.RescaleSlope + dcm_data.RescaleIntercept

# ウィンドウ処理 (Window Level: 50, Window Width: 400)
window_level = 50
window_width = 400
a = window_level - window_width // 2
b = window_level + window_width // 2

processed_img = 255.0 * (img.astype(np.float64) - a) / (b - a)
processed_img = processed_img.clip(0, 255)

# 8bitグレースケール画像として保存
processed_img = processed_img.astype(np.uint8)
pil_image = Image.fromarray(processed_img)
pil_image.save("24TB4039_windowing.png")
