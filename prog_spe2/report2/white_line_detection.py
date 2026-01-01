import cv2
import numpy as np
import matplotlib.pyplot as plt

# 画像読み込み
img = cv2.imread('mitoma.png') # ファイル名は適宜変更してください

# HSV色空間へ変換
hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

# ==========================================================
# 【レポートの要点】 ゴールと白線を区別する色閾値の設定
# ==========================================================
# 解説:
# ゴールポストは直射日光を受けて非常に明るく（白飛びに近い）、
# 明度(V)が 240〜255 の範囲に分布していました。
# 一方、芝生上の白線は少し暗く、240以下に収まっています。
# そこで、明度の上限(V)を 240 に設定することで、ゴールを除外しました。

lower_white = np.array([0, 0, 160]) 
upper_white = np.array([180, 50, 240]) # ここを255から240に下げてゴールを除外

mask_white = cv2.inRange(hsv, lower_white, upper_white)

# --- 形状修復 ---
# 白線の途切れを補修する処理（設定は維持）
# kernel_heal = np.ones((2, 2), np.uint8)
# mask_white = cv2.morphologyEx(mask_white, cv2.MORPH_CLOSE, kernel_heal, iterations=1)

# 輪郭抽出
contours, _ = cv2.findContours(mask_white, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

result_img = img.copy()

line_count = 0

for cnt in contours:
    area = cv2.contourArea(cnt)
    
    # ノイズ除去
    # ゴールは色指定で消えているため、残ったある程度の大きさの塊は「白線」とみなす
    if area < 3000:
        continue

    x, y, w, h = cv2.boundingRect(cnt)

    # --- 白線（Line）として描画 ---
    # 明度フィルタリングの結果、ゴールは検出されず、白線のみが残ることを示す
    cv2.drawContours(result_img, [cnt], -1, (0, 0, 255), 3) # 赤色で描画
    cv2.putText(result_img, "Line", (x-90, y+50), cv2.FONT_HERSHEY_SIMPLEX, 1.0, (0, 0, 255), 2)
    line_count += 1

# 結果の保存・表示
plt.figure(figsize=(8, 10))

# 1. マスク画像（ゴールが消えていることを確認するため）
plt.subplot(2, 1, 1)
plt.imshow(mask_white, cmap='gray')
plt.title("White Mask (V_max=240, Goal Removed)")
plt.axis('off')

# 2. 結果画像（白線のみが赤く囲まれていること）
plt.subplot(2, 1, 2)
plt.imshow(cv2.cvtColor(result_img, cv2.COLOR_BGR2RGB))
plt.title(f"Result: Line Detection (Count={line_count})")
plt.axis('off')

plt.savefig('white_line_detection.png', dpi=300, bbox_inches='tight')
plt.close()