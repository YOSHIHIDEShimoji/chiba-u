import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from sklearn.model_selection import train_test_split, ParameterGrid
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import roc_curve, auc, confusion_matrix, accuracy_score, recall_score


def evaluate_validation(data, label, model):
    probability = model.predict_proba(data)[:, 1]

    # ROC解析
    fpr, tpr, thresholds = roc_curve(label, probability)
    auc_value = auc(fpr, tpr)

    # Youden's indexによるカットオフ値
    cutoff = thresholds[-1]

    return {
        "auc": auc_value,
        "cutoff": cutoff
    }


def evaluate_test(data, label, model, cutoff=0.5):
    probability = model.predict_proba(data)[:, 1]

    # ROC解析
    fpr, tpr, thresholds = roc_curve(label, probability)
    auc_value = auc(fpr, tpr)

    # 混同行列から性能指標を算出
    pred = None
    accuracy = 0
    sensitivity = 0
    specificity = 0
           
    # ROC曲線の描画 (matplotlibを使用)
    plt.plot(fpr, tpr, 'b', linewidth=2.0, clip_on=False)
    base_points = [0.0, 1.0]
    plt.plot(base_points, base_points, 'k', linestyle='dotted')
    plt.xlabel('False positive rate (FPR)')
    plt.ylabel('True positive rate (TPR)')
    plt.xlim(0, 1)
    plt.ylim(0, 1)
    plt.plot(1-specificity, sensitivity, 'o', ms=10, markeredgewidth=2,
             markerfacecolor="None", markeredgecolor='k')
    plt.text(0.5, 0.3, 'AUC=%.3f' % (auc_value), size=15)
    plt.gca().set_aspect('equal', adjustable='box')
    plt.show()

    return {
        "auc": auc_value,
        "accuracy": accuracy,
        "sensitivity": sensitivity,
        "specificity": specificity,
        "cutoff": cutoff
    }


def training_with_parameter_search(train_data, train_label,
                                   val_data, val_label,
                                   hyper_params=None):
    best_auc = -np.inf
    best_model = None
    best_params = None
    best_cutoff = None

    # 試行するパラメータのリストを作成
    param_sets = []

    for params in param_sets:

        # 学習
        model = None
        model.fit(train_data, train_label)
    
        # 検証データによる評価
        val_results = evaluate_validation(val_data, val_label, model)

        # 検証データでの性能がこれまでで最も高ければ、保存するモデル、カットオフを更新
        if val_results["auc"] > best_auc:
            best_auc = val_results["auc"]
            best_model = model
            best_params = params
            best_cutoff = val_results["cutoff"]

    return best_model, best_params, best_cutoff


def main():
    
    in_data_file_name = "diabetes_selected.pkl"
    diabetes_data_selected = pd.read_pickle(in_data_file_name) 

    # データを特徴量(x)とラベル(y)に分割
    diabetes_data = diabetes_data_selected.drop(columns='Outcome')
    diabetes_label = diabetes_data_selected['Outcome']

    # 学習、検証、テストデータに分割 (6:2:2)
    train_data = train_label = val_data = val_label = None
    test_data = test_label = None

    # ハイパーパラメータの設定
    hyper_params = {}

    best_model, best_params, best_cutoff = training_with_parameter_search(train_data,
                                                                          train_label,
                                                                          val_data,
                                                                          val_label,
                                                                          hyper_params)
    print(f"BestParams: {best_params}")

    test_results = evaluate_test(test_data, test_label, best_model, best_cutoff)
    print(test_results)


if __name__ == "__main__":
    main() 