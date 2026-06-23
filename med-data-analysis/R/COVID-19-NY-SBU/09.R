library(pROC)

NewTest <- readRDS("NewTest.rds")

ROC_data <- roc(
  response = NewTest$Disease,
  predictor = NewTest$Value
)

# 課題1
auc(ROC_data)

# 課題2
plot(
  ROC_data,
  identity = TRUE,                    # AUC=0.5の基準線
  print.auc = TRUE,                   # AUCを表示
  print.thres = "best",               # 最適カットオフ値を表示
  print.thres.best.method = "closest.topleft",
  legacy.axes = TRUE                  # 横軸をFPRにする
)

