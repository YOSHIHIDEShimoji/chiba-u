# 演習1
COVID_data <- readRDS("COVID_data_selected.rds")
bartlett.test(respiratory_rate ~ age_splits, data = COVID_data)
summary(aov(respiratory_rate ~ age_splits, data = COVID_data))
oneway.test(respiratory_rate ~ age_splits, data = COVID_data, var.equal = FALSE)
TukeyHSD(aov(respiratory_rate ~ age_splits, data = COVID_data))
pairwise.t.test(COVID_data$respiratory_rate, COVID_data$age_splits, p.adjust.method = "holm", pool.sd = FALSE)
