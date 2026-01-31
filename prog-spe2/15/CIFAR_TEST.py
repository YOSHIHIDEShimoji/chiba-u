from __future__ import print_function

import cv2
import matplotlib.pyplot as plt
import numpy as np

from keras.models import Sequential
from keras.layers import Dense, Dropout, Flatten
from keras.layers import Conv2D, MaxPooling2D

batch_size = 128
num_classes = 10

img_rows, img_cols = 32, 32
img_color = 3

input_shape = (img_rows, img_cols, img_color)

model = Sequential()
model.add(Conv2D(32, kernel_size=(3, 3),
                 activation='relu',
                 input_shape=input_shape))
model.add(Conv2D(64, (3, 3), activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Dropout(0.25))
model.add(Flatten())
model.add(Dense(128, activation='relu'))
model.add(Dropout(0.5))
model.add(Dense(num_classes, activation='softmax'))
model.load_weights('CIFAR_param.weights.h5')


I = cv2.imread('Cat.png')
Data = cv2.cvtColor(I, cv2.COLOR_BGR2RGB)
plt.imshow(Data)
plt.show()

# 修正
Input = Data.astype("float32") / 255.0
Input = Input.reshape(1, 32, 32, 3)

Input = Data.reshape(1,32,32,3)
L = model.predict(Input)
np.set_printoptions(suppress=True)
print(L.T)
print(np.argmax(L))