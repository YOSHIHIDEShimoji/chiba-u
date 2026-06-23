# データファイル読み込み
COVID_data_org <- read.csv("covid-19-ny-sbu-01.csv")
COVID_data_org

# データフレームの操作
COVID_data_org[1,]
COVID_data_org[,7]
length_of_stay <- COVID_data_org$length_of_stay
length_of_stay

# データのカテゴリ化
COVID_data_org$sex <- factor(COVID_data_org$sex, levels = c("MALE", "FEMALE"))
COVID_data_org$sex
COVID_data_org$sex <- factor(COVID_data_org$sex, 
                             levels = c("MALE", "FEMALE"), 
                             labels = c("m", "f"))
COVID_data_org$sex

# データフレームの要約
summary(COVID_data_org)

# 欠損値(NA)を含むデータの解析
# mean(COVID_data_org$oral_temperature)
mean(COVID_data_org$oral_temperature, na.rm=TRUE)

# NA含むケースの削除
# COVID_data <- COVID_data_org[complete.cases(COVID_data_org),]
COVID_data <- na.omit(COVID_data_org)
dim(COVID_data)

# オブジェクトの保存
saveRDS(COVID_data, file="COVID_data_selected.rds")

# ヒストグラムの描画
hist(COVID_data$length_of_stay, 
     breaks = seq(0, 100, 2),
     ylim = c(0, 400),
     main = "Length of stay",
     xlab = "Number of day")

# 箱ひげ図の描画
boxplot(COVID_data$respiratory_rate, 
        ylim = c(0, 60),
        ylab = "Respiratory rate [/min]")

boxplot(saturation ~ was_ventilated, 
        data = COVID_data,
        ylim = c(50, 100),
        xlab = "Was ventilated", 
        ylab = "Oxygen saturation [%]")

# 課題1
# png("boxplot.png", width = 800, height = 600)

boxplot(length_of_stay ~ is_icu,
        data = COVID_data,
        ylim = c(0, 100),
        xlab = "Is ICU",
        ylab = "Length of Stay [days]"
        )

is_icu_means <- tapply(COVID_data$length_of_stay, 
                COVID_data$is_icu,
                mean,
                na.rm = TRUE)
# means
points(x = 1:2,
       y = is_icu_means, 
       pch = 4, 
       cex = 1.5, 
       lwd = 2)


# dev.off()

# 課題2
sum(is.na(COVID_data$is_icu))
count_is_na <- function(x) {
  sum(is.na(x))
}
sapply(COVID_data_org, count_is_na)


