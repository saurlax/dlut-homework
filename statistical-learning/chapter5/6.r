library(ISLR)
library(boot)

data(Default)
set.seed(1)

# 1) 用 glm 拟合并查看解析标准误
fit <- glm(default ~ income + balance, data = Default, family = binomial)
# summary(fit)
coef(summary(fit))[, "Std. Error"]   # glm 的标准误

# 2) 编写 bootstrap 函数返回系数
boot.fn <- function(data, indices) {
  d <- data[indices, ]
  coef(glm(default ~ income + balance, data = d, family = binomial))
}


# 3) 运行 bootstrap
boot.res <- boot(Default, boot.fn, R = 100)

# 4) 输出 bootstrap 标准误并比较
boot.se <- apply(boot.res$t, 2, sd)
names(boot.se) <- names(coef(fit))
cbind(glm.se = coef(summary(fit))[, "Std. Error"],
      boot.se = boot.se)