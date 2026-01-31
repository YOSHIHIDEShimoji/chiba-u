import cv2
import numpy as np
import matplotlib.pyplot as plt

# 画像読み込み
img = cv2.imread('mitoma.png')
hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

# ボール検出
lower_white = np.array([0, 0, 240])  # V = 200 にすると白線が反応してしまう
upper_white = np.array([180, 60, 255])

mask_white = cv2.inRange(hsv, lower_white, upper_white)

# クロージング：ボール内部の空白を埋める
kernel_fill = np.ones((9, 9), np.uint8)
mask_white = cv2.morphologyEx(mask_white, cv2.MORPH_CLOSE, kernel_fill, iterations=1)

# オープニング：ボールと白線の接続部を断ち切る
kernel_cut = np.ones((5, 5), np.uint8)
mask_white = cv2.morphologyEx(mask_white, cv2.MORPH_OPEN, kernel_cut, iterations=2)

# 輪郭抽出
contours, _ = cv2.findContours(mask_white, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

result_img = img.copy()

ball_count = 0

for cnt in contours:
    area = cv2.contourArea(cnt)

    # 小さすぎる領域を無視
    if area < 3000:
        continue

    # 形状計算
    perimeter = cv2.arcLength(cnt, True)

    circularity = 4 * np.pi * area / (perimeter * perimeter)
    x, y, w, h = cv2.boundingRect(cnt)

    # 円に近いかを示す指標
    is_round = (circularity > 0.6)

    if is_round:
        cv2.drawContours(result_img, [cnt], -1, (255, 0, 0), 3)
        cv2.putText(result_img, "BALL", (x-90, y+50),
                    cv2.FONT_HERSHEY_SIMPLEX, 1.0, (255, 0, 0), 3)
        ball_count += 1

# 表示
plt.figure(figsize=(8, 10))

plt.subplot(1, 2, 1)
plt.imshow(mask_white, cmap='gray')
plt.title("Ball Enhanced Mask")
plt.axis('off')

plt.subplot(1, 2, 2)
plt.imshow(cv2.cvtColor(result_img, cv2.COLOR_BGR2RGB))
plt.title(f"Ball (count={ball_count})")
plt.axis('off')

plt.savefig('ball_detection.png', dpi=300, bbox_inches='tight')
plt.close()


# 白線検出
lower_white = np.array([0, 0, 160])
upper_white = np.array([180, 50, 240])  # V = 255 にするとゴールが反応してしまう

mask_white = cv2.inRange(hsv, lower_white, upper_white)

# 輪郭抽出
contours, _ = cv2.findContours(mask_white, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

result_img = img.copy()

line_count = 0

for cnt in contours:
    area = cv2.contourArea(cnt)

    # 小さすぎる領域を無視
    if area < 3000:
        continue

    x, y, w, h = cv2.boundingRect(cnt)

    cv2.drawContours(result_img, [cnt], -1, (0, 0, 255), 3)
    cv2.putText(result_img, "Line", (x-90, y+50),
                cv2.FONT_HERSHEY_SIMPLEX, 1.0, (0, 0, 255), 2)
    line_count += 1

# 表示
plt.figure(figsize=(8, 10))

plt.subplot(1, 2, 1)
plt.imshow(mask_white, cmap='gray')
plt.title("White Line Enhanced Mask")
plt.axis('off')

plt.subplot(1, 2, 2)
plt.imshow(cv2.cvtColor(result_img, cv2.COLOR_BGR2RGB))
plt.title(f"Line Detection (Count={line_count})")
plt.axis('off')

plt.savefig('white_line_detection.png', dpi=300, bbox_inches='tight')
plt.close()
