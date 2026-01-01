import cv2
import numpy as np
import matplotlib.pyplot as plt

# 画像読み込み
img = cv2.imread('mitoma.png') 
hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

# --- 色検出設定 ---
# ボールを逃さないよう、明るさの上限は255（MAX）に戻してしっかり拾います
# ゴールも拾ってしまいますが、あとで「形」で弾くので大丈夫です
# V = 200 にすると白線が反応してしまう。
# lower_white = np.array([0, 0, 200]) 
lower_white = np.array([0, 0, 240]) 
upper_white = np.array([180, 60, 255])

mask_white = cv2.inRange(hsv, lower_white, upper_white)

# --- 形状修復（ここがボール専用チューニング） ---

# 1. 強力な「クロージング」でボールの柄を埋める
# サイズを (15, 15) に設定。これでボール内部のスカスカが埋まります。
kernel_fill = np.ones((9, 9), np.uint8)
mask_white = cv2.morphologyEx(mask_white, cv2.MORPH_CLOSE, kernel_fill, iterations=1)

# 2. 強力な「オープニング」でラインとの癒着を切る
# サイズを (11, 11) に設定。これでボールとラインの接続部（くびれ）を断ち切ります。
kernel_cut = np.ones((5, 5), np.uint8)
mask_white = cv2.morphologyEx(mask_white, cv2.MORPH_OPEN, kernel_cut, iterations=2)

# 輪郭抽出
contours, _ = cv2.findContours(mask_white, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

result_img = img.copy()

ball_count = 0
for cnt in contours:
    area = cv2.contourArea(cnt)
    
    # 小さすぎるゴミは無視
    if area < 2000: continue

    # 1. その輪郭だけのマスクを作る
    mask_single_object = np.zeros(mask_white.shape, dtype=np.uint8)
    cv2.drawContours(mask_single_object, [cnt], -1, 255, -1)
    
    # 2. HSV画像の V(明度)チャンネルの平均値を計算
    # cv2.mean は (H, S, V, 0) の順で平均値を返します
    mean_color = cv2.mean(hsv, mask=mask_single_object)
    mean_brightness = mean_color[2] # 3番目の値が明るさ(V)

    # 形状計算
    perimeter = cv2.arcLength(cnt, True)
    if perimeter == 0: continue
    
    circularity = 4 * np.pi * area / (perimeter * perimeter)
    x, y, w, h = cv2.boundingRect(cnt)
    aspect_ratio = float(w) / h

    # ==============================================================
    # デバッグ表示（数値を見て調整するため）
    # debug_label = f"V:{int(mean_brightness)} C:{circularity:.2f} R:{aspect_ratio:.2f}"
    # 全ての白い塊に数値を黒文字で書く
    # cv2.putText(result_img, debug_label, (x, y-10), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 0), 2)
    # cv2.drawContours(result_img, [cnt], -1, (0, 0, 255), 2)
    # ==============================================================

    # --- ボール判定（ラインやゴールを無視する条件） ---
    
    # 1. 円形度: 0.5以上 (ラインは細長いので0.2とかになる。ゴールも角張ってるので低い)
    is_round = (circularity > 0.5)
    
    # 2. アスペクト比: ほぼ正方形 (0.7 ~ 1.3)
    is_square_box = (0.7 < aspect_ratio < 1.3)
    
    if is_round and is_square_box:
        # 条件に合うものだけを「ボール」として青で囲む
        cv2.drawContours(result_img, [cnt], -1, (255, 0, 0), 3) 
        cv2.putText(result_img, "BALL", (x-90, y+50), cv2.FONT_HERSHEY_SIMPLEX, 1.0, (255, 0, 0), 3)
        ball_count += 1

# 表示
plt.figure(figsize=(8, 10))

plt.subplot(2, 1, 1)
plt.imshow(mask_white, cmap='gray')
plt.title("Ball Enhanced Mask")
plt.axis('off')

plt.subplot(2, 1, 2)
plt.imshow(cv2.cvtColor(result_img, cv2.COLOR_BGR2RGB))
plt.title(f"Result: Ball (count={ball_count})")
plt.axis('off')

plt.savefig('ball_detection.png', dpi=300, bbox_inches='tight')
plt.close()