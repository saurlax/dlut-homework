library(ISLR)

data(Auto)
n <- nrow(Auto)

# (a) 作出所有变量的散点图矩阵
pairs(Auto) # 或 pairs(Auto[ , sapply(Auto, is.numeric)]) 只画数值变量

# (b) 计算变量之间的相关系数矩阵（排除 name 变量）
numVars <- Auto[, sapply(Auto, is.numeric)]
corMat <- cor(numVars)
print(corMat)

# (c) 用 lm() 进行多元线性回归，除 name 外所有变量预测 mpg
fit <- lm(mpg ~ . - name, data = Auto)
summary(fit)

# 输出显著系数（p < 0.05）
coefs <- summary(fit)$coefficients
print(coefs[coefs[, 4] < 0.05,, drop = FALSE])

# 查看 year 的系数
print(coef(fit)["year"])

# (d) 画诊断图并识别异常点与高杠杆/高影响点
par(mfrow = c(2, 2))
plot(fit) # 包含：残差-拟合、正态 QQ、尺度位置、杠杆图
par(mfrow = c(1, 1))

# 计算 Cook's distance 并标记可能的高影响点
cd <- cooks.distance(fit)
plot(cd, type = "h", main = "Cook's distance")
abline(h = 4 / (n - length(coef(fit))), col = "red", lty = 2)
which(cd > 4 / (n - length(coef(fit)))) # 返回高影响点索引
# 也可查看高杠杆点: which(hatvalues(fit) > 2*mean(hatvalues(fit)))

# (e) 使用交互项（: 和 *）拟合，检查是否存在显著交互
# 例：对所有预测变量两两交互 (可能会产生很多项)
fit_inter <- lm(mpg ~ (. - name) ^ 2, data = Auto) # ^2 表示包含主效应和两两交互
sum_inter <- summary(fit_inter)
sum_inter

# 提取交互项并筛选显著的（p < 0.05）
inter_terms <- coef(sum_inter)
inter_idx <- grep(":", rownames(inter_terms))
if (length(inter_idx) > 0) print(inter_terms[inter_idx,, drop = FALSE][inter_terms[inter_idx, 4] < 0.05,, drop = FALSE]) else print("无交互项。")

# (f) 对预测变量尝试不同变换并比较模型
fit_log <- lm(mpg ~ log(displacement) + log(horsepower) + log(weight) + cylinders + year + origin, data = Auto)
fit_sqrt <- lm(mpg ~ sqrt(displacement) + sqrt(horsepower) + sqrt(weight) + cylinders + year + origin, data = Auto)
fit_sq <- lm(mpg ~ I(displacement ^ 2) + I(horsepower ^ 2) + I(weight ^ 2) + cylinders + year + origin, data = Auto)

summary(fit_log)
summary(fit_sqrt)
summary(fit_sq)

# 用 AIC 比较模型优劣
AIC(fit, fit_log, fit_sqrt, fit_sq)

# 也可用交叉验证或 RMSE 等更严格比较，这里给出残差标准差作为快速比较
sapply(list(orig = fit, log = fit_log, sqrt = fit_sqrt, sq = fit_sq), function(m) sigma(m))
