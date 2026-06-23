# 散布図作成
COVID_data <- readRDS("COVID_data_selected.rds")

plot(COVID_data$oral_temperature, COVID_data$heart_rate,
     type = "p",
     pch = 16,
     col = "blue",
     xlab = "oral_temperature [°C]",
     ylab = "heart_rate [bpm]")

regression_result <- lm(COVID_data$heart_rate ~ COVID_data$oral_temperature)

abline(regression_result)

# 問題1
regression_result <- lm(COVID_data$heart_rate ~ COVID_data$oral_temperature)
regression_summary <- summary(regression_result)
cor

# 問題2
regression_summary$coefficients[1, 1]

# 問題3
regression_summary$coefficients[2, 1]

# 問題4
regression_summary$r.squared