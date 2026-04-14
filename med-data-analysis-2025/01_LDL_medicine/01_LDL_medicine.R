LDL_data <- read.csv("24TB4039_LDL_medicine.csv")

LDL_data$sex <- factor(LDL_data$sex, levels = c(1, 2), labels = c("m", "f"))
LDL_data$medicine <- factor(LDL_data$medicine, levels = c("A", "B"))

summary(LDL_data)

tx <- table(LDL_data$sex, LDL_data$medicine)
addmargins(tx)
prop.table(tx)

tapply(LDL_data$age, LDL_data$sex, mean)
tapply(LDL_data$LDL_C, list(LDL_data$sex, LDL_data$medicine), mean)