from __future__ import print_function
import os
import cv2
import numpy as np
from tensorflow import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout, Flatten
from keras.layers import Conv2D, MaxPooling2D

# パラメータの設定
num_classes = 10
img_rows, img_cols = 28, 28
img_color = 1
input_shape = (img_rows, img_cols, img_color)

# モデルの構造を定義
model = Sequential()
model.add(Conv2D(32, kernel_size=(3, 3), activation='relu', input_shape=input_shape))
model.add(Conv2D(64, (3, 3), activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))

# 畳み込み層とプーリング層を追加
model.add(Conv2D(64, (3, 3), activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))

model.add(Dropout(0.25))
model.add(Flatten())
model.add(Dense(128, activation='relu'))
model.add(Dropout(0.5))
model.add(Dense(num_classes, activation='softmax'))

# 重みを読み込む
model.load_weights('MNIST_Simple_e20.weights.h5')

# ディレクトリ指定
img_dir = "data"
files = sorted(os.listdir(img_dir))

# 数字ごとの集計用リスト
correct_per_class = [0] * 10
total_per_class = [0] * 10

for filename in files:
    if not filename.endswith(".png"):
        continue
    
    # ファイル名から正解ラベルを取得
    true_label = int(filename.split('_')[0])

    filepath = os.path.join(img_dir, filename)

    # 画像読み込みと前処理
    I = cv2.imread(filepath, cv2.IMREAD_GRAYSCALE)
    Input = I.astype("float32") / 255.0
    Input = Input.reshape(1, 28, 28, 1)

    # 予測
    L = model.predict(Input, verbose=0)
    pred_label = np.argmax(L)
    
    # 全体のカウント
    total_per_class[true_label] += 1
    
    # 正解ならその数字のカウントを増やす
    is_correct = (pred_label == true_label)
    if is_correct:
        correct_per_class[true_label] += 1
        mark = "OK"
    else:
        mark = "NG"
    
    print(f"[{mark}] File: {filename} -> Pred: {pred_label} (True: {true_label})")

# 各数字の正答率を表示
print("\n--- 正答率の内訳 ---")
for i in range(10):
    if total_per_class[i] > 0:
        acc = (correct_per_class[i] / total_per_class[i]) * 100
        print(f"数字 [{i}]: {acc:.2f}% ({correct_per_class[i]}/{total_per_class[i]})")
    else:
        print(f"数字 [{i}]: データなし")

# 全体の正答率
total_correct = sum(correct_per_class)
total_images = sum(total_per_class)
if total_images > 0:
    overall_acc = (total_correct / total_images) * 100
    print(f"\n全体の正答率: {overall_acc:.2f}% ({total_correct}/{total_images})")