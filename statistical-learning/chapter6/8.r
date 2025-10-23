# 设随机种子并生成数据
set.seed(1)
n <- 100
X <- rnorm(n)                     # 预测变量 X
eps <- rnorm(n)                   # 噪声 ε

# (b) 按多项式模型生成响应 Y = β0 + β1 X + β2 X^2 + β3 X^3 + ε
beta0 <- 1; beta1 <- 2; beta2 <- -1; beta3 <- 0.5
Y <- beta0 + beta1*X + beta2*X^2 + beta3*X^3 + eps
dat <- data.frame(Y, X)
for (k in 2:10) dat[[paste0("X", k)]] <- X^k  # 构造 X^2,...,X^10

# (c) 最优子集选择
library(leaps)
regfit <- regsubsets(Y ~ ., data = dat, nvmax = 10)
s <- summary(regfit)
cp.sel <- which.min(s$cp)
bic.sel <- which.min(s$bic)
adjr.sel <- which.max(s$adjr2)
print(paste("cp.sel =", cp.sel))
print(paste("bic.sel =", bic.sel))
print(paste("adjr.sel =", adjr.sel))
print("coefficients (bic.sel):")
print(coef(regfit, bic.sel))

# 绘制 Cp / BIC / Adjusted R^2 随模型大小变化
par(mfrow = c(2,2))
plot(s$cp, xlab = "Num Variables", ylab = "Cp", type = "b",
	main = "Mallows Cp vs #vars")
points(cp.sel, s$cp[cp.sel], col = "red", pch = 19)

plot(s$bic, xlab = "Num Variables", ylab = "BIC", type = "b",
	main = "BIC vs #vars")
points(bic.sel, s$bic[bic.sel], col = "red", pch = 19)

plot(s$adjr2, xlab = "Num Variables", ylab = "Adjusted R2", type = "b",
	main = "Adjusted R2 vs #vars")
points(adjr.sel, s$adjr2[adjr.sel], col = "red", pch = 19)

par(mfrow = c(1,1))

# (d) 前向和后向逐步选择
reg.fwd <- regsubsets(Y ~ ., data = dat, nvmax = 10, method = "forward")
reg.bwd <- regsubsets(Y ~ ., data = dat, nvmax = 10, method = "backward")
sf <- summary(reg.fwd); sb <- summary(reg.bwd)
print(paste("forward bic select =", which.min(sf$bic)))
print(paste("backward bic select =", which.min(sb$bic)))

# (e) Lasso 与交叉验证
library(glmnet)
xmat <- model.matrix(Y ~ ., data = dat)
cv.lasso <- cv.glmnet(xmat, dat$Y, alpha = 1)
print(paste("lambda.min =", cv.lasso$lambda.min))
plot(cv.lasso)
print("Lasso coefficients at lambda.min:")
print(coef(cv.lasso, s = cv.lasso$lambda.min))

# (f) 使用 Y = beta0 + beta7 * X^7 + eps 检验方法
beta7 <- 3
Y2 <- beta0 + beta7 * X^7 + eps
dat2 <- dat; dat2$Y <- Y2

reg2 <- regsubsets(Y ~ ., data = dat2, nvmax = 10)
s2 <- summary(reg2)
print(paste("bic select for Y2 =", which.min(s2$bic)))
print("coefficients (bic) for Y2:")
print(coef(reg2, which.min(s2$bic)))

cv.lasso2 <- cv.glmnet(model.matrix(Y ~ . -1, data = dat2), dat2$Y, alpha = 1)
print(paste("lasso lambda.min for Y2 =", cv.lasso2$lambda.min))
title(main = "Lasso CV MSE (Y2) vs log(lambda)")
print("Lasso coefficients for Y2 at lambda.min:")
print(coef(cv.lasso2, s = cv.lasso2$lambda.min))