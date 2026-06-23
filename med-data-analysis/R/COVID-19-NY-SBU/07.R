# 演習1, 2
disease_data <- matrix(c(58, 32, 8,
                         24, 40, 12),
                       nrow = 2, byrow = TRUE)

rownames(disease_data) <- c("男性", "女性")
colnames(disease_data) <- c("脳梗塞", "脳出血", "くも膜下出血")

chisq.test(disease_data)

# 演習3
COVID_data <- readRDS("COVID_data_selected.rds")
print("=== 1. last_status ===")
chisq.test(table(COVID_data$sex, COVID_data$last_status))

print("=== 2. age_splits ===")
chisq.test(table(COVID_data$sex, COVID_data$age_splits))

print("=== 3. was_ventilated ===")
chisq.test(table(COVID_data$sex, COVID_data$was_ventilated))

print("=== 4. is_icu ===")
chisq.test(table(COVID_data$sex, COVID_data$is_icu))