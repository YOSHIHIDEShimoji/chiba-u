from __future__ import print_function
import os
import cv2
import numpy as np
from tensorflow import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout, Flatten, Conv2D, MaxPooling2D

# 'simple', 'complex', 'more_complex' から選択
model_type = 'simple'

num_classes = 10
img_rows, img_cols = 28, 28
input_shape = (img_rows, img_cols, 1)

# simple モデル
def build_simple_model(input_shape, num_classes):
    model = Sequential()
    model.add(Conv2D(32, kernel_size=(3, 3), activation='relu', input_shape=input_shape))
    model.add(Conv2D(64, (3, 3), activation='relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Conv2D(64, (3, 3), activation='relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Dropout(0.25))
    model.add(Flatten())
    model.add(Dense(128, activation='relu'))
    model.add(Dropout(0.5))
    model.add(Dense(num_classes, activation='softmax'))
    return model

# complex モデル
def build_complex_model(input_shape, num_classes):
    model = Sequential()
    model.add(Conv2D(64, (3, 3), activation='relu', padding='same', input_shape=input_shape))
    model.add(Conv2D(64, (3, 3), activation='relu', padding='same'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Conv2D(128, (3, 3), activation='relu', padding='same'))
    model.add(Conv2D(128, (3, 3), activation='relu', padding='same'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Conv2D(256, (3, 3), activation='relu', padding='same'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Flatten())
    model.add(Dense(1024, activation='relu'))
    model.add(Dropout(0.5))
    model.add(Dense(512, activation='relu'))
    model.add(Dropout(0.5))
    model.add(Dense(num_classes, activation='softmax'))
    return model

# more_complex Model
def build_more_complex_model(input_shape, num_classes):
    model = Sequential()
    model.add(Conv2D(128, (3, 3), activation='relu', padding='same', input_shape=input_shape))
    model.add(Conv2D(128, (3, 3), activation='relu', padding='same'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Conv2D(256, (3, 3), activation='relu', padding='same'))
    model.add(Conv2D(256, (3, 3), activation='relu', padding='same'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Conv2D(512, (3, 3), activation='relu', padding='same'))
    model.add(Conv2D(512, (3, 3), activation='relu', padding='same'))
    model.add(Conv2D(512, (3, 3), activation='relu', padding='same'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Flatten())
    model.add(Dense(4096, activation='relu'))
    model.add(Dropout(0.5))
    model.add(Dense(4096, activation='relu'))
    model.add(Dropout(0.5))
    model.add(Dense(num_classes, activation='softmax'))
    return model

# モデル構築
print(f'model type: {model_type}')

if model_type == 'simple':
    model = build_simple_model(input_shape, num_classes)
elif model_type == 'complex':
    model = build_complex_model(input_shape, num_classes)
elif model_type == 'more_complex':
    model = build_more_complex_model(input_shape, num_classes)

# 重み読み込み
weights_path = f'./models/MNIST_{model_type}.weights.h5'
model.load_weights(weights_path)

# 画像判定処理
img_dir = 'data'
files = sorted(os.listdir(img_dir))

# 数字ごとの集計リスト
correct_per_class = [0] * 10
total_per_class = [0] * 10

for filename in files:
    if not filename.endswith('.png'):
        continue

    # ファイル名から正解を取得
    true_label = int(filename.split('_')[0])
    filepath = os.path.join(img_dir, filename)

    # 画像読み込みと前処理
    I = cv2.imread(filepath, cv2.IMREAD_GRAYSCALE)
    Input = I.astype('float32') / 255.0
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
        mark = 'OK'
    else:
        mark = 'NG'
    
    print(f'[{mark}] File: {filename} -> Pred: {pred_label} (True: {true_label})')

# 各数字の正答率を表示
print('\n--- Result ---')
for i in range(10):
    if total_per_class[i] > 0:
        acc = (correct_per_class[i] / total_per_class[i]) * 100
        print(f'Digit {i}: {acc:.2f}% ({correct_per_class[i]}/{total_per_class[i]})')
        
# 全体の正答率
total_correct = sum(correct_per_class)
total_images = sum(total_per_class)
print(f'\nTotal Accuracy: {total_correct / total_images * 100:.2f}%')