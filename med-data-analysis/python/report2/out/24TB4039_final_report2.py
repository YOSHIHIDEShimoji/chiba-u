import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from sklearn.model_selection import train_test_split, ParameterSampler
from sklearn.ensemble import RandomForestClassifier, GradientBoostingClassifier
from sklearn.metrics import roc_curve, auc, confusion_matrix, accuracy_score, recall_score


def evaluate_validation(data, label, model):
    probability = model.predict_proba(data)[:, 1]

    # ROC解析
    fpr, tpr, thresholds = roc_curve(label, probability)
    auc_value = auc(fpr, tpr)

    # Youden's indexによるカットオフ値
    cutoff = thresholds[np.argmax(tpr - fpr)]

    return {
        "auc": auc_value,
        "cutoff": cutoff
    }


def evaluate_test(data, label, model, cutoff=0.5, out_file_name=None):
    probability = model.predict_proba(data)[:, 1]

    # ROC解析
    fpr, tpr, thresholds = roc_curve(label, probability)
    auc_value = auc(fpr, tpr)

    # 混同行列から性能指標を算出
    pred = (probability >= cutoff).astype(int)
    tn, fp, fn, tp = confusion_matrix(label, pred).ravel()
    accuracy = accuracy_score(label, pred)
    sensitivity = recall_score(label, pred)
    specificity = tn / (tn + fp)

    # ROC曲線の描画 (matplotlibを使用)
    plt.figure()
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
    if out_file_name is not None:
        plt.savefig(out_file_name, dpi=200, bbox_inches='tight')
    plt.show()

    return {
        "auc": auc_value,
        "accuracy": accuracy,
        "sensitivity": sensitivity,
        "specificity": specificity,
        "cutoff": cutoff,
        "confusion_matrix": {"tn": tn, "fp": fp, "fn": fn, "tp": tp}
    }


def training_with_parameter_search(train_data, train_label,
                                   val_data, val_label,
                                   hyper_params=None,
                                   model_class=RandomForestClassifier):
    best_auc = -np.inf
    best_model = None
    best_params = None
    best_cutoff = None

    # 試行するパラメータのリストを作成 (ランダム探索)
    param_sets = list(ParameterSampler(hyper_params, n_iter=20, random_state=45678))

    for params in param_sets:

        # 学習
        model = model_class(random_state=23456, **params)
        model.fit(train_data, train_label)

        # 検証データによる評価
        val_results = evaluate_validation(val_data, val_label, model)

        # 検証データでの性能がこれまでで最も高ければ、保存するモデル、カットオフを更新
        if val_results["auc"] > best_auc:
            best_auc = val_results["auc"]
            best_model = model
            best_params = params
            best_cutoff = val_results["cutoff"]

    print(f"ValidationAUC: {best_auc:.3f}")
    return best_model, best_params, best_cutoff


def show_feature_importance(model, feature_names, out_file_name=None):
    # ランダムフォレストの特徴量の重要度 (ジニ不純度の減少量) を降順に描画
    importance = pd.Series(model.feature_importances_, index=feature_names)
    importance = importance.sort_values()
    print(importance.sort_values(ascending=False))

    plt.figure()
    plt.barh(importance.index, importance.values, color='b')
    plt.xlabel('Feature importance')
    plt.tight_layout()
    if out_file_name is not None:
        plt.savefig(out_file_name, dpi=200, bbox_inches='tight')
    plt.show()


def main():

    # Load data
    csv_file_name = "covid-19-ny-sbu-03.csv"
    covid_all_data = pd.read_csv(csv_file_name, index_col=0)

    # 前処理：欠損値を含む症例を除外
    print(f"読み込んだ症例数: {len(covid_all_data)}")
    print(covid_all_data.isna().sum())
    covid_all_data = covid_all_data.dropna()
    print(f"欠損値除外後の症例数: {len(covid_all_data)}")

    # データを特徴量(x)とラベル(y)に分割
    covid_data = covid_all_data.drop(columns="last_status")
    covid_label = covid_all_data["last_status"]

    # 学習用データとテストデータに分割(random_stateは変更しないこと!!)
    trainval_data, test_data, trainval_label, test_label = train_test_split(covid_data,
                                                                      covid_label,
                                                                      test_size=0.2,
                                                                      random_state=260715)
    train_data, val_data, train_label, val_label = train_test_split(trainval_data,
                                                                    trainval_label,
                                                                    test_size=0.25,
                                                                    random_state=260715)

    for name, label in [("学習", train_label), ("検証", val_label), ("テスト", test_label)]:
        counts = label.value_counts()
        print(f"{name}データ: 退院 {counts.get(0, 0)}例, 死亡 {counts.get(1, 0)}例")

    # ランダムフォレストのハイパーパラメータの探索範囲
    hyper_params = {"n_estimators": range(1, 21),
                    "max_depth": range(1, 6),
                    "criterion": ["gini", "entropy"],
                    "min_samples_leaf": [1, 2, 4]}

    best_model, best_params, best_cutoff = training_with_parameter_search(train_data,
                                                                          train_label,
                                                                          val_data,
                                                                          val_label,
                                                                          hyper_params)

    print(f"BestParams: {best_params}")

    test_results = evaluate_test(test_data, test_label, best_model, best_cutoff,
                                 out_file_name="roc_rf.png")
    print(test_results)

    # 特徴量の重要度の評価
    show_feature_importance(best_model, covid_data.columns,
                            out_file_name="importance.png")

    # 比較：勾配ブースティング (演習で扱っていない識別器)
    gb_hyper_params = {"n_estimators": [50, 100, 200],
                       "max_depth": range(1, 6),
                       "learning_rate": [0.01, 0.05, 0.1, 0.2],
                       "min_samples_leaf": [1, 2, 4]}

    gb_model, gb_params, gb_cutoff = training_with_parameter_search(train_data,
                                                                    train_label,
                                                                    val_data,
                                                                    val_label,
                                                                    gb_hyper_params,
                                                                    GradientBoostingClassifier)

    print(f"BestParams(GB): {gb_params}")

    gb_results = evaluate_test(test_data, test_label, gb_model, gb_cutoff,
                               out_file_name="roc_gb.png")
    print(gb_results)


if __name__ == "__main__":
    main()
