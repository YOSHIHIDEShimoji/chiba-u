import cv2
import matplotlib.pyplot as plt
import numpy as np
from scipy.interpolate import interp1d
from skimage.filters import gaussian
from skimage.segmentation import active_contour, chan_vese
from sklearn.cluster import MeanShift, estimate_bandwidth

OrgImg = cv2.imread('DotsImg.png', cv2.IMREAD_GRAYSCALE)

# 二値化
thresh, BnImg = cv2.threshold(OrgImg, 0, 255, cv2.THRESH_OTSU)
print('thresh =', thresh)
plt.imshow(BnImg, cmap='gray')
plt.show()

# k-means(グレースケール)
W, H = np.shape(OrgImg)
K = 2
TmpImg = OrgImg.reshape(-1, 1).astype(np.float32)
criteria = (cv2.TERM_CRITERIA_EPS + cv2.KMEANS_RANDOM_CENTERS, 30, 1.0)
ret, label, center = cv2.kmeans(TmpImg, K, None, criteria, 10, cv2.KMEANS_RANDOM_CENTERS)
center = np.uint8(center)
RsltImg = center[label.flatten()]
RsltImg = RsltImg.reshape((W, H))
plt.imshow(RsltImg)
plt.show()

# k-means(カラー)
ColorImg = cv2.imread('SampleImg.bmp')
W, H, C = np.shape(ColorImg)
K = 16
TmpImg = ColorImg.reshape((-1, 3)).astype(np.float32)
criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 1.0)
ret, label, center = cv2.kmeans(TmpImg, K, None, criteria, 10, cv2.KMEANS_RANDOM_CENTERS)
center = np.uint8(center)
Rslt = center[label.flatten()]
RsltImg = Rslt.reshape((W, H, C))
RsltImg = cv2.cvtColor(RsltImg, cv2.COLOR_BGR2RGB)
plt.imshow(RsltImg)
plt.show()

# MeanShift
Img1D = np.reshape(OrgImg, (-1, 1))
BandWidth = estimate_bandwidth(Img1D, quantile=0.2, n_samples=100)
RsltMS = MeanShift(bandwidth=BandWidth, bin_seeding=True)
RsltMS.fit(Img1D)
MSImg = RsltMS.labels_
MSImg = np.reshape(MSImg, OrgImg.shape)
print('分離数 =', len(set(RsltMS.labels_)))
plt.imshow(MSImg)
plt.show()

# Watershed
thresh, BnImg = cv2.threshold(OrgImg, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)
Dist = cv2.distanceTransform(BnImg, cv2.DIST_L2, 5)
ret, FgImg = cv2.threshold(Dist, 0.5 * Dist.max(), 255, 0)
kernel = np.ones((3, 3), np.uint8)
BgImg = cv2.dilate(BnImg, kernel, iterations=2)
FgImg = np.uint8(FgImg)
unknown = cv2.subtract(BgImg, FgImg)

ret, markers = cv2.connectedComponents(FgImg)
markers = markers + 1
markers[unknown==255] = 0

OrgImg_C = cv2.cvtColor(OrgImg, cv2.COLOR_GRAY2BGR)
markers = cv2.watershed(OrgImg_C, markers)
plt.imshow(markers)
plt.show()

# Chan-Vese
GrayImg = cv2.imread('DotsImg.png', cv2.IMREAD_GRAYSCALE)
cv = chan_vese(GrayImg, 
               mu=0.25, 
               lambda1=1, 
               lambda2=1, 
               tol=1e-3, 
               max_num_iter=100, 
               dt=0.5, 
               init_level_set='checkerboard', 
               extended_output=True)
fig, axes = plt.subplots(1, 2, figsize=(8, 8))
ax = axes.flatten()
ax[0].imshow(GrayImg)
ax[0].set_axis_off()
ax[1].imshow(cv[0])
ax[1].set_axis_off()
plt.show()

# Snake法
s = np.linspace(0, 2 * np.pi, 800)
x = 128 + 100 * np.cos(s)
y = 215 + 100 * np.sin(s)
init = np.array([y, x]).T

snake = active_contour(gaussian(OrgImg, 3), 
                       init, 
                       boundary_condition='periodic',
                       alpha=0.015,
                       beta=10, 
                       gamma=0.001)
fig, ax = plt.subplots()
ax.imshow(OrgImg, 'gray', vmin=0, vmax=255)
ax.plot(init[:, 1], init[:, 0], '--r', lw=3)
ax.plot(snake[:, 1], snake[:, 0], ':b', lw=3)
plt.show()

