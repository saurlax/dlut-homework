library(MASS)
library(boot)

data(Boston)
medv <- Boston$medv
n <- length(medv)
set.seed(1)

# (a) 总体均值的估计 mu_hat
mu_hat <- mean(medv)

# (b) 均值的解析标准误（样本标准差 / sqrt(n)）
se_analytic <- sd(medv) / sqrt(n)

# (c) 用自助法(bootstrap)估计均值的标准误
boot_mean <- boot(medv, statistic = function(x, i) mean(x[i]), R = 2000)
se_boot_mean <- sd(boot_mean$t)   # bootstrap 标准误
# 也可用 boot::boot.ci 获得 CI
ci_boot_mean <- boot.ci(boot_mean, type = c("norm","basic","perc","bca"))

# (d) 用自助法得到的均值 95% 置信区间，并与 t.test 比较
ci_ttest <- t.test(medv)$conf.int
# 近似的 ±2SE 区间
ci_approx <- c(mu_hat - 2*se_analytic, mu_hat + 2*se_analytic)

# (e)-(f) 中位数估计及其标准误（解析式没有简单公式，使用 bootstrap）
median_hat <- median(medv)
boot_median <- boot(medv, statistic = function(x, i) median(x[i]), R = 2000)
se_boot_median <- sd(boot_median$t)
ci_boot_median <- boot.ci(boot_median, type = c("perc","bca"))

# (g)-(h) 10% 分位数的估计及 bootstrap 标准误
p10_hat <- as.numeric(quantile(medv, 0.10))
boot_p10 <- boot(medv, statistic = function(x, i) quantile(x[i], 0.10), R = 2000)
se_boot_p10 <- sd(boot_p10$t)
ci_boot_p10 <- boot.ci(boot_p10, type = c("perc","bca"))

# 输出主要结果
cat("n =", n, "\n\n")

cat("(a) mean (mu_hat) =", round(mu_hat,4), "\n")
cat("(b) analytic SE =", round(se_analytic,4), "\n\n")

cat("(c) bootstrap SE (mean) =", round(se_boot_mean,4), "\n")
cat("    bootstrap mean CIs (from boot.ci):\n")
print(ci_boot_mean)
cat("\n(d) t.test 95% CI for mean =", round(ci_ttest,4), "\n")
cat("    approx +/-2SE CI =", round(ci_approx,4), "\n\n")

cat("(e) median (median_hat) =", round(median_hat,4), "\n")
cat("(f) bootstrap SE (median) =", round(se_boot_median,4), "\n")
cat("    bootstrap median CIs:\n"); print(ci_boot_median); cat("\n")

cat("(g) 10% quantile (p10_hat) =", round(p10_hat,4), "\n")
cat("(h) bootstrap SE (10% quantile) =", round(se_boot_p10,4), "\n")
cat("    bootstrap 10%-quantile CIs:\n"); print(ci_boot_p10)