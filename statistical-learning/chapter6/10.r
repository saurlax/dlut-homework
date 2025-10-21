set.seed(1)

# 生成 X (n x p) 和稀疏的 beta
n <- 1000; p <- 20
X <- matrix(rnorm(n * p), nrow = n, ncol = p)
true_beta <- c(3, -2, 1.5, 0, 0, 2, rep(0, p - 6))  # 自定一些非零，其他为0 (长度 p)
eps <- rnorm(n, sd = 1)
Y <- X %*% true_beta + eps

# 划分训练(100) / 测试(900)
train_idx <- sample(1:n, 100)
test_idx  <- setdiff(1:n, train_idx)
X_train <- X[train_idx, , drop = FALSE]; Y_train <- Y[train_idx]
X_test  <- X[test_idx, , drop = FALSE];  Y_test  <- Y[test_idx]

# 载入包并做最优子集选择
library(leaps)
# 构造训练数据框带列名以便 regsubsets 使用
train_df <- as.data.frame(X_train)
colnames(train_df) <- paste0("X", 1:p)
train_df$Y <- as.numeric(Y_train)
# 在训练集上做 regsubsets，nvmax = p
regfit <- regsubsets(Y ~ ., data = train_df, nvmax = p, method = "exhaustive")
regsum <- summary(regfit)

# 辅助函数：用 regsubsets 对新数据预测
predict_regsubsets <- function(object, newdata, id) {
  coefs <- coef(object, id = id)
  vars <- names(coefs)
  # 构造模型矩阵（包含截距）
  Xnew <- as.data.frame(newdata)
  colnames(Xnew) <- paste0("X", 1:ncol(Xnew))
  Xmat <- cbind("(Intercept)" = 1, Xnew)
  # 保证列的顺序与系数匹配
  Xmat_sub <- Xmat[, vars, drop = FALSE]
  as.numeric(as.matrix(Xmat_sub) %*% as.numeric(coefs))
}

# 计算每个模型大小 k 的训练 MSE（使用 regsubsets 在训练集选出的最佳模型）和测试 MSE
train_mse_k <- numeric(p)
test_mse_k  <- numeric(p)
beta_dist_k <- numeric(p)  # ||beta - beta_hat_r||_2

for (k in 1:p) {
  # 训练集上 regsubsets 已给出最佳 k 大小模型，取得系数并预测
  pred_train_k <- predict_regsubsets(regfit, X_train, id = k)
  pred_test_k  <- predict_regsubsets(regfit, X_test, id = k)
  train_mse_k[k] <- mean((Y_train - pred_train_k)^2)
  test_mse_k[k]  <- mean((Y_test - pred_test_k)^2)
  # 构造 beta_hat 向量（长度 p），不存在的变量系数为 0
  coef_k <- coef(regfit, id = k)
  beta_hat <- rep(0, p)
  # coef names like "(Intercept)", "X1", ...
  coef_names <- names(coef_k)
  for (nm in coef_names) {
    if (nm != "(Intercept)") {
      idx <- as.integer(sub("X", "", nm))
      beta_hat[idx] <- coef_k[nm]
    }
  }
  beta_dist_k[k] <- sqrt(sum((true_beta - beta_hat)^2))
}

# 输出并画图（英文标签）
print(sprintf("Best model size by minimal test MSE: %d", which.min(test_mse_k)))
print(sprintf("Test MSEs (k=1..p): %s", paste(round(test_mse_k, 3), collapse = ", ")))

# 绘图：test MSE vs model size
plot(1:p, test_mse_k, type = "b", pch = 19, xlab = "Model size (k)", ylab = "Test MSE",
     main = "Test MSE vs Model Size")
# 绘图：distance ||beta - beta_hat|| vs model size
plot(1:p, beta_dist_k, type = "b", pch = 19, xlab = "Model size (k)", ylab = "||beta - beta_hat||_2",
     main = "Beta Distance vs Model Size")

# 打印最优模型的系数
best_k <- which.min(test_mse_k)
print(sprintf("Best k = %d", best_k))
print("Coefficients of best model (select subset):")
print(coef(regfit, id = best_k))