# 题 10：Boston 数据集（MASS 包）。

# (a) 载入数据，报告维度与含义
library(MASS)
data("Boston")
cat("(a) 维度:", nrow(Boston), "行 x", ncol(Boston), "列；行=城镇/普查区，列=变量（含响应 medv）。\n")

# (b) 预测变量的成对散点图（不含 medv）
preds <- setdiff(names(Boston), "medv")
pairs(Boston[, preds], main = "(b) 预测变量散点矩阵")

# (c) 与 medv 的关系：相关系数（降序）
cors <- sort(cor(Boston$medv, Boston[, preds]), decreasing = TRUE)
cat("(c) medv 与各预测变量相关（降序）：\n")
print(round(cors, 3))

# (d) 简单线性回归 medv ~ lstat 并作图
plot(Boston$lstat, Boston$medv, pch = 19, col = "grey40",
     xlab = "lstat", ylab = "medv", main = "(d) medv ~ lstat")
fit_lm <- lm(medv ~ lstat, data = Boston)
abline(fit_lm, col = "red", lwd = 2)
cat("(d) 回归摘要：\n"); print(summary(fit_lm)$coefficients)

# (e) 紧邻查尔斯河的城镇数量（chas=1）
n_chas <- sum(Boston$chas == 1)
cat("(e) chas=1 的数量：", n_chas, "\n")

# (f) 各城镇师生比的中位数
cat("(f) ptratio 的中位数：", median(Boston$ptratio), "\n")

# (g) medv 最低的城镇及其其余变量
idx_min <- which(Boston$medv == min(Boston$medv))
cat("(g) medv 最低值为：", min(Boston$medv), "；对应行索引：", paste(idx_min, collapse = ", "), "\n")
print(Boston[idx_min, ])

# (h) 平均房间数 rm > 7 与 > 8 的数量；>8 子集特征摘要
n_rm7 <- sum(Boston$rm > 7)
n_rm8 <- sum(Boston$rm > 8)
cat("(h) rm>7 的数量：", n_rm7, "；rm>8 的数量：", n_rm8, "\n")
cat("(h) rm>8 子集关键变量摘要（rm, medv, lstat, crim, tax, ptratio）：\n")
print(summary(Boston[Boston$rm > 8, c("rm","medv","lstat","crim","tax","ptratio")]))