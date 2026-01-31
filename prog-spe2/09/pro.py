import os
import PIL, cv2, skimage
from skimage import io

input_dir = "InputPictures"
output_dir = "OutputPictures"

os.makedirs(output_dir, exist_ok=True)

files = os.listdir(input_dir)

for name in files:
    path = os.path.join(input_dir, name)
    out_path = os.path.join(output_dir, name)

    # cv2
    Img = cv2.imread(path)

    if Img is None:
        print("skip:", name)
        continue

    Img_RGB = cv2.cvtColor(Img, cv2.COLOR_BGR2RGB)

    # skimage 読み込み
    img = io.imread(path)

    # 保存
    io.imsave(out_path, img)

    print("saved:", out_path)
