set.seed(1)

# 数据生成
n <- 100
x1 <- rnorm(n)
x2 <- rnorm(n)
beta0_true <- 1.0
beta1_true <- 2.0
beta2_true <- -1.0
y <- beta0_true + beta1_true * x1 + beta2_true * x2 + rnorm(n, sd = 1)

# 多元线性回归（作为比较基准）
full <- lm(y ~ x1 + x2)
coef_full <- coef(full)  # (Intercept), x1, x2

# 交替拟合参数初始化
iters <- 1000
beta0_rec <- numeric(iters)
beta1_rec <- numeric(iters)
beta2_rec <- numeric(iters)

beta2 <- runif(1, -1, 1)   # 随机初始化 β2（题目要求可任意）
beta1 <- 0
beta0 <- 0

for(i in 1:iters){
  # (c) 固定 beta2，拟合 a = y - beta2*x2 = beta0 + beta1*x1 + eps
  a1 <- y - beta2 * x2
  fit1 <- lm(a1 ~ x1)
  beta0 <- coef(fit1)[1]
  beta1 <- coef(fit1)[2]

  # (d) 固定 beta1，拟合 a = y - beta1*x1 = beta0 + beta2*x2 + eps
  a2 <- y - beta1 * x1
  fit2 <- lm(a2 ~ x2)
  beta0 <- coef(fit2)[1]
  beta2 <- coef(fit2)[2]

  # 存储每次迭代结果（记录在完成一次(c)+(d)之后）
  beta0_rec[i] <- beta0
  beta1_rec[i] <- beta1
  beta2_rec[i] <- beta2
}

# 输出最后几次估计值作为检查
cat("最后 5 次迭代的估计（beta0, beta1, beta2）：\n")
print(tail(data.frame(beta0=beta0_rec, beta1=beta1_rec, beta2=beta2_rec), 5))

matplot(1:iters, cbind(beta0_rec, beta1_rec, beta2_rec), type = "l", lwd = 2,
        col = c("black", "red", "blue"), lty = 1,
        xlab = "iteration", ylab = "estimate",
        main = "交替拟合：beta0 (黑), beta1 (红), beta2 (蓝)")
legend("topright", legend = c("beta0","beta1","beta2"), col = c("black","red","blue"), lwd=2)

# 叠加多元回归的估计（水平虚线）
abline(h = coef_full["(Intercept)"], col = "black", lty = 2)
abline(h = coef_full["x1"], col = "red", lty = 2)
abline(h = coef_full["x2"], col = "blue", lty = 2)

abline(h = beta0_true, col = "gray50", lty = 3)
abline(h = beta1_true, col = "gray50", lty = 3)
abline(h = beta2_true, col = "gray50", lty = 3)

# 比较收敛值与多元回归结果
cat("\n多元回归系数 (lm):\n"); print(coef_full)
cat("\n交替拟合最后一次估计:\n"); print(c(beta0 = beta0, beta1 = beta1, beta2 = beta2))