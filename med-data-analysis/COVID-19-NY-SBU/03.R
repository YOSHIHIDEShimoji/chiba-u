# 演習1
pnorm(150, mean = 125, sd = 15, lower.tail = F)

# 演習2
sample_data <- c(2.9, 5.7, 8.8, 4.4, 6.3, 4.8, 5.2)
sample_mean <- mean(sample_data)
lower_limit <- sample_mean - 1.96 * 2 / sqrt(length(sample_data))
lower_limit

upper_limit <- sample_mean + 1.96 * 2 / sqrt(length(sample_data))
upper_limit

# 演習3
sample_sd <- sd(sample_data)
t_limit <- qt(0.975, length(sample_data) - 1)
lower_limit <- sample_mean - t_limit * sample_sd / sqrt(length(sample_data))
lower_limit

upper_limit <- sample_mean + t_limit * sample_sd / sqrt(length(sample_data))
upper_limit

t.test(sample_data)

# 問題1
prob_1 <- pnorm(1.55, mean = 0, sd = 1, lower.tail = F)
prob_1

# 問題2
hights <- c(128, 132, 121, 133, 121, 123, 124, 136, 132, 144)
hights_mean <- mean(hights)
lower_limit <- hights_mean - 1.96 * 6 / sqrt(length(hights))
lower_limit

# 問題3
upper_limit <- hights_mean + 1.96 * 6 / sqrt(length(hights))
upper_limit

# 問題4
hights_sd <- sd(hights)
t_limit <- qt(0.975, length(hights) - 1)
lower_limit <- hights_mean - t_limit * hights_sd / sqrt(length(hights))
lower_limit

upper_limit <- hights_mean + t_limit * hights_sd / sqrt(length(hights))
upper_limit




