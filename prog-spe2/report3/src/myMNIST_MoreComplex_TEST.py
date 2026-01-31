from __future__ import print_function
import os
import cv2
import numpy as np
from tensorflow import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout, Flatten, Conv2D, MaxPooling2D

# パラメータの設定
num_classes = 10
img_rows, img_cols = 28, 28
input_shape = (img_rows, img_cols, 1)

model = Sequential()

# 1 つめの層
model.add(Conv2D(128, (3, 3), activation='relu', padding='same', input_shape=input_shape))
model.add(Conv2D(128, (3, 3), activation='relu', padding='same'))
model.add(MaxPooling2D(pool_size=(2, 2)))

# 2 つめの層
model.add(Conv2D(256, (3, 3), activation='relu', padding='same'))
model.add(Conv2D(256, (3, 3), activation='relu', padding='same'))
model.add(MaxPooling2D(pool_size=(2, 2)))

# 3 つめの層
model.add(Conv2D(512, (3, 3), activation='relu', padding='same'))
model.add(Conv2D(512, (3, 3), activation='relu', padding='same'))
model.add(Conv2D(512, (3, 3), activation='relu', padding='same'))
model.add(MaxPooling2D(pool_size=(2, 2)))

# 4つめの層
model.add(Flatten())
model.add(Dense(4096, activation='relu'))
model.add(Dropout(0.5))
model.add(Dense(4096, activation='relu'))
model.add(Dropout(0.5))
model.add(Dense(num_classes, activation='softmax'))

# 学習済み重みの読み込み
model.load_weights('MNIST_MoreComplex_e100.weights.h5')

# ディレクトリ指定
img_dir = "data"
files = sorted(os.listdir(img_dir))

# 数字ごとの集計用リスト
correct_per_class = [0] * 10
total_per_class = [0] * 10

print(f"判定開始: MNIST_MoreComplex_e100 モデルを使用\n")

for filename in files:
    if not filename.endswith(".png"):
        continue
    
    # ファイル名から正解を取得
    try:
        true_label = int(filename.split('_')[0])
    except:
        continue

    filepath = os.path.join(img_dir, filename)

    # 画像読み込みと前処理
    I = cv2.imread(filepath, cv2.IMREAD_GRAYSCALE)
    if I is None:
        continue
        
    Input = I.astype("float32") / 255.0
    Input = Input.reshape(1, 28, 28, 1)

    # 予測実行
    L = model.predict(Input, verbose=0)
    pred_label = np.argmax(L)

    # 全体のカウント
    total_per_class[true_label] += 1

    is_correct = (pred_label == true_label)
    
    # 正解ならその数字のカウントを増やす
    if is_correct:
        correct_per_class[true_label] += 1
        mark = "OK"
    else:
        mark = "NG"
    
    print(f"[{mark}] File: {filename} -> Pred: {pred_label} (True: {true_label})")

# 結果表示
print("\n--- 正答率の内訳 ---")
for i in range(10):
    if total_per_class[i] > 0:
        acc = (correct_per_class[i] / total_per_class[i]) * 100
        print(f"数字 [{i}]: {acc:.2f}% ({correct_per_class[i]}/{total_per_class[i]})")

total_correct = sum(correct_per_class)
total_images = sum(total_per_class)
if total_images > 0:
    print(f"\n全体の正答率: {total_correct / total_images * 100:.2f}% ({total_correct}/{total_images})")