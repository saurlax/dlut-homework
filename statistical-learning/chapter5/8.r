library(boot)

# 1) 用 seed=1 生成模拟数据并画散点
set.seed(1)
x <- rnorm(100)
y <- x - 2*x^2 + rnorm(100)
dat <- data.frame(x,y)
plot(x, y, main="Scatter: x vs y", xlab="x", ylab="y")

# 2) 用最小二乘拟合 4 个模型（多项式到 4 次）
fit1 <- glm(y ~ x,                data=dat)   # degree 1
fit2 <- glm(y ~ x + I(x^2),       data=dat)   # degree 2
fit3 <- glm(y ~ x + I(x^2) + I(x^3), data=dat) # degree 3
fit4 <- glm(y ~ x + I(x^2) + I(x^3) + I(x^4), data=dat) # degree 4

# 3) 计算 LOOCV 误差，显式设 K = nrow(dat)
loocv <- function(fit, data) cv.glm(data, fit, K = nrow(data))$delta[1]
cv.errs.seed1 <- c(
  degree1 = loocv(fit1, dat),
  degree2 = loocv(fit2, dat),
  degree3 = loocv(fit3, dat),
  degree4 = loocv(fit4, dat)
)
print("LOOCV errors (seed=1):")
print(cv.errs.seed1)

# 4) 用另一个随机种子重复步骤并比较
set.seed(2)
x2 <- rnorm(100)
y2 <- x2 - 2*x2^2 + rnorm(100)
dat2 <- data.frame(x=x2, y=y2)

fit1b <- glm(y ~ x, data=dat2)
fit2b <- glm(y ~ x + I(x^2), data=dat2)
fit3b <- glm(y ~ x + I(x^2) + I(x^3), data=dat2)
fit4b <- glm(y ~ x + I(x^2) + I(x^3) + I(x^4), data=dat2)

cv.errs.seed2 <- c(
  degree1 = loocv(fit1b, dat2),
  degree2 = loocv(fit2b, dat2),
  degree3 = loocv(fit3b, dat2),
  degree4 = loocv(fit4b, dat2)
)
print("LOOCV errors (seed=2):")
print(cv.errs.seed2)

# 5) 哪个模型 LOOCV 最小（对两次种子分别判断）
cat("Best model (seed=1):", names(which.min(cv.errs.seed1)), "\n")
cat("Best model (seed=2):", names(which.min(cv.errs.seed2)), "\n")

# 6) 输出每个模型的系数以便比较估计统计量
cat("\nCoefficients (seed=1):\n")
print(lapply(list(fit1,fit2,fit3,fit4), coef))
cat("\nSummary (seed=1) for degree-2 model (典型示例):\n")
print(summary(fit2))
