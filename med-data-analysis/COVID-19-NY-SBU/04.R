# 演習1
sample_data <- c(2.9, 5.7, 8.8, 4.4, 6.3, 4.8, 5.2)
t.test(sample_data, mu = 6.0)
sample_mean <- mean(sample_data)
sample_sd <- sd(sample_data)
t_start <- (sample_mean - 6.0) / (sample_sd / sqrt(length(sample_data)))
df <- length(sample_data) - 1
p <- pt(t_start, df) + pt(-t_start, df, lower.tail = F)
p

# 演習2
LDL_before <- c(158, 148, 132, 107, 142)
LDL_after <- c(134, 135, 116, 110, 127)
t.test(LDL_before, LDL_after, paired = TRUE)

# 演習3
COVID_data <- readRDS("COVID_data_selected.rds")
t.test(saturation ~ was_ventilated, data = COVID_data)

# 課題2
heights <- c(128, 132, 121, 133, 121, 123, 124, 136, 132, 144)
t.test(heights, mu = 135, alternative = "two.sided")

# 課題3
t.test(systolic_blood_pressure ~ is_icu, data = COVID_data)
t.test(oral_temperature ~ is_icu, data = COVID_data)
t.test(length_of_stay ~ is_icu, data = COVID_data)
t.test(heart_rate ~ is_icu, data = COVID_data)
