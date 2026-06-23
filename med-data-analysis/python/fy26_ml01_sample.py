import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from sklearn.model_selection import train_test_split
from sklearn.svm import SVC
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import roc_curve, auc


def do_evaluation(label, probability):

    # ROC解析
    fpr, tpr, thresholds = roc_curve(label, probability)
    auc_value = auc(fpr, tpr)

    # ROC曲線の描画 (matplotlibを使用)
    plt.plot(fpr, tpr, 'b', linewidth=2.0, clip_on=False)
    base_points = [0.0, 1.0]
    plt.plot(base_points, base_points, 'k', linestyle='dotted')
    plt.xlabel('False positive rate (FPR)')
    plt.ylabel('True positive rate (TPR)')
    plt.xlim(0, 1)
    plt.ylim(0, 1)
    plt.text(0.5, 0.3, 'AUC=%.3f' % (auc_value), size=15)
    plt.gca().set_aspect('equal', adjustable='box')
    plt.show()

    return auc_value


def do_training(data, label):
    
    model = None

    return model


def do_test(data, label, model):

    probability = None

    auc_value = do_evaluation(label, probability)

    return  auc_value


def main():
    
    in_data_file_name = "diabetes_selected.pkl"
    diabetes_data_selected = pd.read_pickle("diabetes_selected.pkl") 

    # データを特徴量(x)とラベル(y)に分割
    diabetes_data = diabetes_data_selected
    diabetes_label = diabetes_data_selected

    # 学習用データとテストデータに分割

    model = do_training(train_data, train_label)

    auc_value = do_test(test_data, test_label, model)
    print(auc_value)


if __name__ == "__main__":
    main()    