set.seed(1)
library(e1071)

# (a) 生成 p=2 的二类数据（近线性可分）
n <- 500
x1 <- rnorm(n)
x2 <- rnorm(n)
# 线性边界上加一点噪声使几乎线性可分
y <- factor(ifelse(x1 + x2 + rnorm(n, sd = 0.4) > 0, "A", "B"))
dat <- data.frame(x1 = x1, x2 = x2, y = y)

# 划分训练与测试
train_idx <- sample(n, 350)
train <- dat[train_idx, ]
test  <- dat[-train_idx, ]

# (b) 对一系列 cost 值计算交叉验证误差（使用 tune.svm）并记录每个 cost 对应训练集上的支持向量数（误分类数）
costs <- c(0.01, 0.1, 1, 10, 100)
cv.err <- numeric(length(costs))
train.err <- numeric(length(costs))
train.svcount <- integer(length(costs))

for (i in seq_along(costs)) {
  C <- costs[i]
  # 使用 5 折交叉验证来估计误差（tune.svm 返回的 performance 为错误率）
  tune.out <- tune.svm(y ~ ., data = train, kernel = "linear",
                       cost = C, tunecontrol = tune.control(cross = 5))
  cv.err[i] <- tune.out$best.performance  # misclassification rate from CV for this cost
  
  # 在整个训练集上用该 cost 拟合并计算训练误差与支持向量数量
  fit <- svm(y ~ ., data = train, kernel = "linear", cost = C, scale = TRUE)
  pred.train <- predict(fit, train)
  train.err[i] <- mean(pred.train != train$y)
  train.svcount[i] <- length(fit$index)
}

# (c) 在测试集上计算每个 cost 的测试误差，找出使测试误差最小的 cost（以及使训练误差最小的 cost）
test.err <- numeric(length(costs))
for (i in seq_along(costs)) {
  fit <- svm(y ~ ., data = train, kernel = "linear", cost = costs[i], scale = TRUE)
  pred.test <- predict(fit, test)
  test.err[i] <- mean(pred.test != test$y)
}

# 汇总结果并输出
res <- data.frame(cost = costs,
                  cv_error = cv.err,
                  train_error = train.err,
                  test_error = test.err,
                  train_SV_count = train.svcount)
print(res)

best_cv <- res$cost[which.min(res$cv_error)]
best_test <- res$cost[which.min(res$test_error)]
best_train <- res$cost[which.min(res$train_error)]

cat("最优（CV 最小）cost =", best_cv, "\n")
cat("最优（测试误差最小）cost =", best_test, "\n")
cat("最优（训练误差最小）cost =", best_train, "\n")

# 可视化（可选）：cost vs 三类误差
matplot(log10(res$cost), res[, c("cv_error","train_error","test_error")], type = "b",
        pch = 1:3, col = 1:3, xlab = "log10(cost)", ylab = "misclassification rate")
legend("topright", legend = c("CV error","Train error","Test error"), col = 1:3, pch = 1:3)