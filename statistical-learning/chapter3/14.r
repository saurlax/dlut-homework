set.seed(1)
x1 <- runif(100)
x2 <- 0.5 * x1 + rnorm(100) / 10
y <- 2 + 2 * x1 + 0.3 * x2 + rnorm(100)

# (a) 用 x1 和 x2 拟合线性模型
fit_full <- lm(y ~ x1 + x2)
cat("=== (a) 全模型 summary ===\n")
print(summary(fit_full))

# (b) 计算相关系数并绘制散点图（图形为英文标签）
cor_x1_x2 <- cor(x1, x2)
cat("\n=== (b) x1 与 x2 的相关系数 ===\n")
print(cor_x1_x2)
plot(x1, x2, main = "Scatterplot of x1 vs x2", xlab = "x1", ylab = "x2") # 英文标题/标签
abline(lm(x2 ~ x1), col = "red")
legend("topleft", legend = "regression line", col = "red", lty = 1, bty = "n")

# (c) 全模型的系数估计并检验显著性
cat("\n=== (c) 全模型系数估计 ===\n")
print(coef(summary(fit_full)))
cat("\n真实系数: beta0=2, beta1=2, beta2=0.3\n")
p_beta1 <- coef(summary(fit_full))["x1", "Pr(>|t|)"]
p_beta2 <- coef(summary(fit_full))["x2", "Pr(>|t|)"]
cat("全模型中 H0: beta1=0 的 p-value =", p_beta1, "，是否拒绝 =", (p_beta1 < 0.05), "\n")
cat("全模型中 H0: beta2=0 的 p-value =", p_beta2, "，是否拒绝 =", (p_beta2 < 0.05), "\n")

# (d) 只用 x1 拟合并检验
fit_x1 <- lm(y ~ x1)
cat("\n=== (d) 只用 x1 的模型 summary ===\n")
print(summary(fit_x1))
cat("只用 x1 时 H0: beta1=0 的 p-value =", coef(summary(fit_x1))["x1", "Pr(>|t|)"], "，是否拒绝 =", (coef(summary(fit_x1))["x1", "Pr(>|t|)"] < 0.05), "\n")

# (e) 只用 x2 拟合并检验
fit_x2 <- lm(y ~ x2)
cat("\n=== (e) 只用 x2 的模型 summary ===\n")
print(summary(fit_x2))
cat("只用 x2 时 H0: beta2=0 的 p-value =", coef(summary(fit_x2))["x2", "Pr(>|t|)"], "，是否拒绝 =", (coef(summary(fit_x2))["x2", "Pr(>|t|)"] < 0.05), "\n")

# (f) 比较模型的 R^2、调整后 R^2 和残差标准差
cmp <- data.frame(
  model = c("full(x1+x2)", "x1 only", "x2 only"),
  R2 = c(summary(fit_full)$r.squared, summary(fit_x1)$r.squared, summary(fit_x2)$r.squared),
  adjR2 = c(summary(fit_full)$adj.r.squared, summary(fit_x1)$adj.r.squared, summary(fit_x2)$adj.r.squared),
  sigma = c(sigma(fit_full), sigma(fit_x1), sigma(fit_x2))
)
cat("\n=== (f) 模型比较 (R2, adjR2, sigma) ===\n")
print(cmp)

# (g) 添加一个新的（可能有测量错误的）观测并重新拟合，检查其影响
x1_new <- c(x1, 0.1)
x2_new <- c(x2, 0.8)
y_new <- c(y, 6)

fit_full_new <- lm(y_new ~ x1_new + x2_new)
fit_x1_new <- lm(y_new ~ x1_new)
fit_x2_new <- lm(y_new ~ x2_new)

cat("\n=== (g) 加入新观测后：全模型 summary ===\n")
print(summary(fit_full_new))
cat("\n加入新观测后：只用 x1 summary ===\n")
print(summary(fit_x1_new))
cat("\n加入新观测后：只用 x2 summary ===\n")
print(summary(fit_x2_new))


# 计算影响度量（杠杆、标准化残差、Cook's distance）
n_new <- length(y_new)
p_full <- length(coef(fit_full_new))
hat_full <- hatvalues(fit_full_new)
stdres_full <- rstandard(fit_full_new)
cooks_full <- cooks.distance(fit_full_new)

cat("\n新观测索引:", n_new, "\n")
cat("新观测的 leverage =", hat_full[n_new], "\n")
cat("新观测的 standardized residual =", stdres_full[n_new], "\n")
cat("新观测的 Cook's distance =", cooks_full[n_new], "\n")

lever_threshold <- 2 * p_full / n_new
cook_threshold <- 4 / (n_new - p_full)
out_stdres <- which(abs(stdres_full) > 2)
out_high_leverage <- which(hat_full > lever_threshold)
out_high_cook <- which(cooks_full > cook_threshold)

cat("\n经验阈值：leverage >", lever_threshold, "；Cook's >", cook_threshold, "\n")
cat("标准化残差绝对值>2 的点索引：", paste(out_stdres, collapse = ", "), "\n")
cat("高杠杆点索引：", paste(out_high_leverage, collapse = ", "), "\n")
cat("高影响点索引（Cook's）：", paste(out_high_cook, collapse = ", "), "\n")

cat("\n新观测的 (x1, x2, y) =", x1_new[n_new], x2_new[n_new], y_new[n_new], "\n")

plot(x1, x2, main = "x1 vs x2 (original data and new obs)", xlab = "x1", ylab = "x2")
points(x1_new[n_new], x2_new[n_new], col = "red", pch = 19)
legend("topleft", legend = "new observation", col = "red", pch = 19, bty = "n")

plot(cooks_full, type = "h", main = "Cook's distance (full model, with new obs)", xlab = "Index", ylab = "Cook's distance")
abline(h = cook_threshold, col = "red", lty = 2)
points(n_new, cooks_full[n_new], col = "red", pch = 19)
