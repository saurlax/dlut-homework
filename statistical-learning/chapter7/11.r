set.seed(1)
n <- 100
x1 <- rnorm(n)
# 让 x2 与 x1 有较强相关性（rho=0.8）
rho <- 0.8
x2 <- rho * x1 + sqrt(1 - rho^2) * rnorm(n)

beta0_true <- 1.0
beta1_true <- 2.0
beta2_true <- -1.0
y <- beta0_true + beta1_true * x1 + beta2_true * x2 + rnorm(n, sd = 1)

full <- lm(y ~ x1 + x2)
coef_full <- coef(full)

iters <- 1000
beta0_rec <- numeric(iters)
beta1_rec <- numeric(iters)
beta2_rec <- numeric(iters)

beta2 <- runif(1, -5, 5)
beta1 <- 0
beta0 <- 0

for(i in 1:iters){
  a1 <- y - beta2 * x2
  fit1 <- lm(a1 ~ x1)
  beta0 <- coef(fit1)[1]
  beta1 <- coef(fit1)[2]

  a2 <- y - beta1 * x1
  fit2 <- lm(a2 ~ x2)
  beta0 <- coef(fit2)[1]
  beta2 <- coef(fit2)[2]

  beta0_rec[i] <- beta0
  beta1_rec[i] <- beta1
  beta2_rec[i] <- beta2
}

print(head(data.frame(beta0=beta0_rec, beta1=beta1_rec, beta2=beta2_rec), 10))
matplot(1:iters, cbind(beta0_rec, beta1_rec, beta2_rec), type = "l", lwd = 2,
        col = c("black", "red", "blue"), lty = 1,
        xlab = "iteration", ylab = "estimate",
        main = "Alternating fit: beta0, beta1, beta2")
legend("topright", legend = c("beta0","beta1","beta2"), col = c("black","red","blue"), lwd=2)
abline(h = coef_full["(Intercept)"], col = "black", lty = 2)
abline(h = coef_full["x1"], col = "red", lty = 2)
abline(h = coef_full["x2"], col = "blue", lty = 2)
abline(h = beta0_true, col = "gray50", lty = 3)
abline(h = beta1_true, col = "gray50", lty = 3)
abline(h = beta2_true, col = "gray50", lty = 3)