library(ISLR); library(gbm); library(glmnet); library(randomForest)
set.seed(1)

# (a) 删除缺失并对 Salary 做对数变换
data(Hitters)
hit <- na.omit(Hitters)
hit$LogSalary <- log(hit$Salary)
hit$Salary <- NULL    # 保留 LogSalary 作为响应

# (b) 划分训练/测试：训练集为 200 个观测，其余为测试
n <- nrow(hit)
train_idx <- sample(n, 200)
train <- hit[train_idx, ]
test  <- hit[-train_idx, ]

# 准备矩阵输入（glmnet 需要）
xtrain <- model.matrix(LogSalary ~ . , data = train)[, -1]
xtest  <- model.matrix(LogSalary ~ . , data = test)[, -1]
ytrain <- train$LogSalary
ytest  <- test$LogSalary

# (c) 用 1000 棵树执行提升（gbm），对若干 shrinkage 值进行比较
n.trees <- 1000
shrinkages <- c(0.001, 0.01, 0.05, 0.1)
test.mse <- numeric(length(shrinkages))
models <- list()

for (i in seq_along(shrinkages)) {
  eta <- shrinkages[i]
  models[[i]] <- gbm(LogSalary ~ .,
                     data = train,
                     distribution = "gaussian",
                     n.trees = n.trees,
                     interaction.depth = 3,
                     shrinkage = eta,
                     n.minobsinnode = 10,
                     verbose = FALSE)
  pred <- predict(models[[i]], newdata = test, n.trees = n.trees)
  test.mse[i] <- mean((ytest - pred)^2)
}

# (d) 绘图：x 轴为 shrinkage，y 轴为对应测试均方误差
plot(shrinkages, test.mse, type = "b", xlab = "shrinkage", ylab = "Test MSE",
     main = "Boosting: shrinkage vs Test MSE")

# (e) 与第 3 章（线性回归）和第 6 章（岭回归/套索）方法比较：计算它们在相同训练/测试划分下的测试 MSE
# 线性回归（OLS）
lm.fit <- lm(LogSalary ~ ., data = train)
lm.pred <- predict(lm.fit, newdata = test)
lm.mse <- mean((ytest - lm.pred)^2)

# 岭回归（glmnet，alpha=0）
cv.ridge <- cv.glmnet(xtrain, ytrain, alpha = 0)
ridge.pred <- predict(cv.ridge, s = "lambda.min", newx = xtest)
ridge.mse <- mean((ytest - ridge.pred)^2)

# 套索（glmnet，alpha=1）
cv.lasso <- cv.glmnet(xtrain, ytrain, alpha = 1)
lasso.pred <- predict(cv.lasso, s = "lambda.min", newx = xtest)
lasso.mse <- mean((ytest - lasso.pred)^2)

# (f) 在提升模型中查看最重要的预测变量（以 shrinkage=0.01 的模型为例）
best.idx <- which.min(test.mse)
best.shrink <- shrinkages[best.idx]
best.gbm <- models[[best.idx]]
summary(best.gbm, n.trees = n.trees)    # 变量重要性

# (g) 对训练集使用装袋（bagging），并计算测试 MSE
p <- ncol(xtrain)
bag.fit <- randomForest(x = xtrain, y = ytrain, mtry = p, ntree = 500, importance = TRUE)
bag.pred <- predict(bag.fit, newdata = xtest)
bag.mse <- mean((ytest - bag.pred)^2)
importance(bag.fit)                      # 变量重要性（bagging）

# 输出比较结果
res <- data.frame(method = c(paste0("gbm_shrink=", shrinkages),
                             "OLS", "Ridge (cv)", "Lasso (cv)", "Bagging"),
                  test_mse = c(test.mse, lm.mse, ridge.mse, lasso.mse, bag.mse))
print(res)
cat("\nBest boosting shrinkage =", best.shrink, "with test MSE =", round(test.mse[best.idx], 4), "\n")