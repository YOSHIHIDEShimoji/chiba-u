# データ読み込み
TG <- c(60, 120, 100, 160, 75, 115, 110, 125, 85, 135, 150, 175, 90, 200)
FBS <- c(72, 88, 82, 97, 76, 84, 86, 91, 74, 90, 95, 101, 80, 105)

# 正規性の確認（Shapiro-Wilk検定）
shapiro.test(TG)
shapiro.test(FBS)

# Pearsonの積率相関係数と無相関検定
cor(TG, FBS)
cor.test(TG, FBS)
