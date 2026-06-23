library(lawstat)
library(coin)

# 問題1
tg_values <- c(238, 123, 225, 168, 248, 124, 145, 218, 136, 189, 363, 225, 451, 264, 185, 165, 352, 459, 336, 258)
tg_groups <- factor(rep(c("A", "B"), c(10, 10)))
tapply(tg_values, tg_groups, shapiro.test)
levene.test(tg_values, tg_groups)

# 問題2
t.test(tg_values ~ tg_groups, var.equal = FALSE)

# 問題3
COVID_data <- readRDS("COVID_data_selected.rds")
COVID_data$is_icu <- factor(COVID_data$is_icu, levels = c("TRUE", "FALSE"))
tapply(COVID_data$oral_temperature, COVID_data$is_icu, shapiro.test)

# 問題4
wilcox_test(oral_temperature ~ is_icu, data = COVID_data, distribution = "exact")


