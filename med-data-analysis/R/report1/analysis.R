cat("\n========== 課題1 ==========\n")
# データ読み込み
A <- c(1.8, 2.1, 2.4, 2.0, 1.9, 2.7, 2.2, 1.7, 2.3, 2.5, 2.1, 2.4)
B <- c(2.5, 1.9, 3.0, 2.6, 2.1, 3.1, 2.0, 2.1, 2.7, 2.3, 2.6, 2.4)

# 正規性の確認（Shapiro-Wilk検定）
shapiro.test(A)
shapiro.test(B)

# 対応のあるt検定
t.test(A, B, paired = TRUE)

cat("\n========== 課題2 ==========\n")
# データ読み込み
A <- c(1.8, 2.1, 2.4, 2.0, 1.9, 2.7, 2.2, 1.7, 2.3, 2.5, 2.1, 2.4)
B <- c(2.5, 1.9, 3.0, 2.6, 2.1, 3.1, 2.0, 2.1, 2.7, 2.3, 2.6, 2.4)

# 正規性の確認（Shapiro-Wilk検定）
shapiro.test(A)
shapiro.test(B)

# 対応のあるt検定
t.test(A, B, paired = TRUE)

cat("\n========== 課題3 ==========\n")
# データ読み込み
smoking_data <- read.csv("smoking_bmi.csv")
smoking_data$smoking <- factor(smoking_data$smoking)

# 正規性確認（Shapiro-Wilk検定）
tapply(smoking_data$bmi, smoking_data$smoking, shapiro.test)

# 等分散性確認（Levene検定）
library(lawstat)
levene.test(smoking_data$bmi, smoking_data$smoking)

# Welchの分散分析
oneway.test(bmi ~ smoking, data = smoking_data, var.equal = FALSE)

# Games-Howell多重比較
library(PMCMRplus)
gamesHowellTest(smoking_data$bmi, smoking_data$smoking)

cat("\n========== 課題4 ==========\n")
# データ読み込み
glucose_data <- read.csv("glucose_monitoring.csv", header = FALSE)

# 正規性確認（Shapiro-Wilk検定）
apply(glucose_data, 2, shapiro.test)

# ANOVA君の読み込み
source("./anovakun_489.txt")

# 球面性確認（Mauchly球面性検定、反復測定による分散分析、関連2群のt検定）
anovakun(glucose_data, "sA", 3, mau = T, auto = T)
