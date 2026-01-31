import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from sklearn.datasets import make_classification
from sklearn.model_selection import train_test_split
from sklearn import datasets, model_selection, svm, metrics
from sklearn.metrics import roc_auc_score
from matplotlib.colors import ListedColormap
import cv2
from sklearn.preprocessing import StandardScaler

samples = make_classification(n_samples=1000, n_features=2, n_informative=1, n_redundant=0, n_classes=2, n_clusters_per_class=1)
X = samples[0] # 特徴量
Y = samples[1] # 分類ラベル

X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=0.2, random_state=None)

plt.scatter(X_train[:,0], X_train[:,1], c=[['c', 'm'][t] for t in Y_train])
plt.scatter(X_test[:,0], X_test[:,1], c=[['b', 'r'][t] for t in Y_test])
plt.show()

def plot_decision_regions(x, y, pre, resolution=0.01):
    cmap = ListedColormap(('blue', 'red'))
    x1_min, x1_max = x[:, 0].min()-1, x[:, 0].max() +1
    x2_min, x2_max = x[:, 1].min()-1, x[:, 1].max() +1
    x1_mesh, x2_mesh = np.meshgrid(np.arange(x1_min, x1_max, resolution), np.arange(x2_min, x2_max, resolution))
    z = clf.predict(np.array([x1_mesh.ravel(), x2_mesh.ravel()]).T)
    z = z.reshape(x1_mesh.shape)
    plt.contourf(x1_mesh, x2_mesh, z, alpha=0.4, cmap=cmap)
    plt.xlim(x1_mesh.min(), x1_mesh.max())
    plt.ylim(x2_mesh.min(), x2_mesh.max())
    plt.scatter(x[: ,0],x[:,1], c=[['b', 'r'][ans] if ans == q else 'y' for ans, q in zip(y, pre)], edgecolors='black')

clf = svm.SVC(gamma='scale') # 分類器SVMを定義
clf.fit(X_train, Y_train) # 学習データを入力し、SVMを学習
pre = clf.predict(X_test) # テストデータで予測

ac_score = metrics.accuracy_score(Y_test, pre)
print('正解率 = {0:.1f}%'.format(ac_score * 100))

plot_decision_regions(X_test, Y_test, pre)
plt.show()

iris = datasets.load_iris()
iris.keys()
iris_data = pd.DataFrame(data=iris.data, columns=iris.feature_names)
iris_data.head()

features = iris.data[:, [0, 2]]

plt.figure(figsize=(5, 5))
plt.title('Iris length data (all:n=150)')
plt.xlabel('sepal length (cm)')
plt.ylabel('petal length (cm)')
plt.scatter(*features.T, c=[['c', 'm', 'y'][x] for x in iris.target], alpha=0.6)
plt.show()

X_train, X_test, label_train, label_test = model_selection.train_test_split(iris.data, iris.target, test_size=0.25, random_state=0)
print('Train:', len(X_train))
print('Test:', len(X_test))

clf = svm.SVC(gamma='auto') # 分類器SVMを定義
clf.fit(X_train, label_train)
pre = clf.predict(X_test) # テストデータで予測

ac_score = metrics.accuracy_score(label_test, pre)
print('正解率 {0:.1f}%'.format(ac_score * 100))

plt.figure(figsize=(5, 5))
features = X_test[:, [0, 2]]
plt.title('Iris length data (test:n=38)')
plt.xlabel('sepal length (cm)')
plt.ylabel('petal length (cm)')

plt.scatter(*features.T, c=[['c', 'm', 'y'][ans] if ans == x else 'r' for ans, x in zip(label_test, pre)], alpha=0.6)
plt.show()

InputData = np.loadtxt("Flabel.csv", delimiter=",")
TrainData = InputData[:, :3]
TrainLabel = InputData[:, 3:].ravel()
sc = StandardScaler() # データ標準化のモジュールを定義
TrainData_std = sc.fit_transform(TrainData)
clf = svm.SVC()
clf.fit(TrainData_std, TrainLabel)

OrgImg = np.array(cv2.imread('SampleImg.bmp'))
[W, H, C] = OrgImg.shape
TestImg = OrgImg.reshape((W*H, C))
Test_std = sc.fit_transform(TestImg.astype(float))
pre = clf.predict(Test_std)
pre = pre.reshape((W, H))
plt.imshow(pre, cmap='gray')
plt.show()