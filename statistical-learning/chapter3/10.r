library(ISLR)
data(Carseats)
str(Carseats)

# (a) 多元回归：Price, Urban, US 预测 Sales
fit1 <- lm(Sales ~ Price + Urban + US, data = Carseats)
summary(fit1)

# (b) 显示模型各系数及因素变量处理方式
contrasts(Carseats$Urban)
contrasts(Carseats$US)
coef(summary(fit1))

# (c) 将模型写成方程（根据估计系数构造字符串）
coefs <- coef(fit1)
intercept <- round(coefs["(Intercept)"], 4)
price_coef <- round(coefs["Price"], 4)
urbanYes <- if ("UrbanYes" %in% names(coefs)) round(coefs["UrbanYes"], 4) else NA
  usYes <- if ("USYes" %in% names(coefs)) round(coefs["USYes"], 4) else NA
  cat("模型: Sales =", intercept, "+", price_coef, "*Price",
    if (!is.na(urbanYes)) paste("+", urbanYes, "*I(Urban=='Yes')") else "",
    if (!is.na(usYes)) paste("+", usYes, "*I(US=='Yes')") else "", "\n")

# (d) 哪些预测变量可以拒绝 H0: beta_j = 0 ?（使用 p < 0.05）
pvals <- coef(summary(fit1))[, "Pr(>|t|)"]
signif_vars <- names(pvals)[which(pvals < 0.05)]
cat("p < 0.05 的变量：", paste(signif_vars, collapse = ", "), "\n")

# (e) 在此基础上用显著的预测变量拟合一个更小的模型
# 下面示例用 Price 和 US（常见的显著组合）；也可根据上面 p 值自动选择
fit_reduced <- lm(Sales ~ Price + US, data = Carseats)
summary(fit_reduced)

# (f) 比较两个模型的拟合：R^2、调整后R^2、AIC、ANOVA 检验
cat("fit1 R2, adjR2:\n");
print(c(R2 = summary(fit1)$r.squared, adjR2 = summary(fit1)$adj.r.squared))
cat("fit_reduced R2, adjR2:\n");
print(c(R2 = summary(fit_reduced)$r.squared, adjR2 = summary(fit_reduced)$adj.r.squared))
cat("AIC 比较:\n");
print(AIC(fit1, fit_reduced))
cat("ANOVA 检验（嵌套模型比较）:\n");
print(anova(fit_reduced, fit1))

# (g) 计算 fit_reduced 中系数的 95% 置信区间
confint(fit_reduced, level = 0.95)

# (h) 检查是否有离群点或高杠杆点：诊断图、Cook's distance、杠杆值
par(mfrow = c(2, 2))
plot(fit_reduced)
par(mfrow = c(1, 1))

n <- nrow(Carseats)
p <- length(coef(fit_reduced))
cd <- cooks.distance(fit_reduced)
lev <- hatvalues(fit_reduced)
outliers <- which(abs(rstandard(fit_reduced)) > 2)
high_cook <- which(cd > 4 / (n - p))
high_lev <- which(lev > 2 * mean(lev))

cat("标准化残差绝对值>2 的点：", paste(outliers, collapse = ", "), "\n")
cat("Cook's distance > 4/(n-p) 的点：", paste(high_cook, collapse = ", "), "\n")
cat("高杠杆点（hat > 2*平均hat）：", paste(high_lev, collapse = ", "), "\n")
