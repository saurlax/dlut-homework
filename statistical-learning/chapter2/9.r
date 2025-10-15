library(ISLR)
data("Auto")
Auto$name   <- factor(Auto$name)
Auto$origin <- factor(Auto$origin)
preds <- setdiff(names(Auto), "mpg")
quant_preds <- preds[sapply(Auto[preds], is.numeric)]
qual_preds  <- setdiff(preds, quant_preds)
cat("(a) 定量预测变量:", paste(quant_preds, collapse = ", "), "\n")
cat("(a) 定性预测变量:", paste(qual_preds,  collapse = ", "), "\n\n")

# (b) 各定量预测变量的取值范围（range）
cat("(b) 各定量预测变量的最小-最大：\n")
print(sapply(Auto[quant_preds], range))

# (c) 各定量预测变量的均值与方差
cat("\n(c) 均值：\n"); print(sapply(Auto[quant_preds], mean))
cat("\n(c) 方差：\n"); print(sapply(Auto[quant_preds], var))

# (d) 删去第 10 与第 85 个观测后，再给出范围、均值、标准差
Auto_sub <- Auto[-c(10, 85), ]
cat("\n(d) 删去 10 与 85 后的最小-最大：\n")
print(sapply(Auto_sub[quant_preds], range))
cat("\n(d) 删去 10 与 85 后的均值：\n")
print(sapply(Auto_sub[quant_preds], mean))
cat("\n(d) 删去 10 与 85 后的标准差：\n")
print(sapply(Auto_sub[quant_preds], sd))

# (e) 与 mpg 的关系
pairs(~ mpg + cylinders + displacement + horsepower + weight + acceleration + year,
      data = Auto, main = "(e) mpg 与主要定量预测变量的散点矩阵")

# (f) 相关性与简单回归以佐证可预测 mpg 的变量
cors <- sort(cor(Auto$mpg, Auto[quant_preds]), decreasing = TRUE)
cat("\n(f) mpg 与各定量预测变量的相关系数（降序）：\n"); print(cors)
fit <- lm(mpg ~ cylinders + displacement + horsepower + weight + acceleration + year, data = Auto)
cat("\n(f) 线性回归显著性摘要（用于佐证哪些变量有用）：\n")
print(summary(fit)$coefficients)