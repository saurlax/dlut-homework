suppressPackageStartupMessages({
  library(ISLR)    # College 数据
  library(glmnet)  # ridge, lasso
  library(pls)     # PCR, PLS
})
set.seed(1)

# (a) 划分训练集/测试集（70%/30%）
data(College)
n <- nrow(College)
train_idx <- sample(seq_len(n), size = floor(0.7 * n))
trainData <- College[train_idx, ]
testData  <- College[-train_idx, ]

# 为 glmnet 构造 x/y 矩阵（去掉截距列）
x_all <- model.matrix(Apps ~ ., data = College)[, -1, drop = FALSE]
y_all <- College$Apps
x_train <- x_all[train_idx, , drop = FALSE]
x_test  <- x_all[-train_idx, , drop = FALSE]
y_train <- y_all[train_idx]
y_test  <- y_all[-train_idx]

# (b) 线性回归（OLS）及测试误差
lm_fit <- lm(Apps ~ ., data = trainData)
pred_lm <- predict(lm_fit, newdata = testData)
mse_lm <- mean((y_test - pred_lm)^2)
print(paste("OLS test MSE =", round(mse_lm, 3)))

# (c) Ridge 回归（交叉验证选 λ）
cv_ridge <- cv.glmnet(x_train, y_train, alpha = 0)
lambda_ridge <- cv_ridge$lambda.min
pred_ridge <- as.numeric(predict(cv_ridge, s = lambda_ridge, newx = x_test))
mse_ridge <- mean((y_test - pred_ridge)^2)
print(paste("Ridge lambda.min =", signif(lambda_ridge, 6)))
print(paste("Ridge test MSE =", round(mse_ridge, 3)))

# (d) Lasso（交叉验证选 λ），输出测试误差与非零系数个数
cv_lasso <- cv.glmnet(x_train, y_train, alpha = 1)
lambda_lasso <- cv_lasso$lambda.min
coef_lasso <- as.matrix(coef(cv_lasso, s = lambda_lasso))
nz_count <- sum(abs(coef_lasso) > 1e-6) - 1  # 去掉截距
pred_lasso <- as.numeric(predict(cv_lasso, s = lambda_lasso, newx = x_test))
mse_lasso <- mean((y_test - pred_lasso)^2)
print(paste("Lasso lambda.min =", signif(lambda_lasso, 6)))
print(paste("Lasso test MSE =", round(mse_lasso, 3)))
print(paste("Number of nonzero coefficients (excluding intercept) =", nz_count))

# (e) PCR（交叉验证选 M），确保选的组件数在 [1, max] 范围内（避免 ncomp() 调用问题）
pcr_fit <- pcr(Apps ~ ., data = trainData, scale = TRUE, validation = "CV")
pcr_msep <- MSEP(pcr_fit, estimate = "CV")
# 获取 max components 的稳健方式：查看 coef 的第三维度或 scores 列数
max_comp_pcr <- if (!is.null(dim(coef(pcr_fit)))) dim(coef(pcr_fit))[3] else if (!is.null(pcr_fit$scores)) ncol(pcr_fit$scores) else 1
idx_pcr <- which.min(pcr_msep$val[1, , 1])       # 索引包含 0 组件
ncomp_pcr <- idx_pcr - 1
ncomp_pcr <- max(1, min(ncomp_pcr, max_comp_pcr)) # 保证至少 1，且不超过最大
pred_pcr <- as.numeric(predict(pcr_fit, newdata = testData, ncomp = ncomp_pcr))
mse_pcr <- mean((y_test - pred_pcr)^2)
print(paste("PCR selected ncomp =", ncomp_pcr))
print(paste("PCR test MSE =", round(mse_pcr, 3)))

# (f) PLS（交叉验证选 M），同样处理以避免 ncomp() 问题
pls_fit <- plsr(Apps ~ ., data = trainData, scale = TRUE, validation = "CV")
pls_msep <- MSEP(pls_fit, estimate = "CV")
max_comp_pls <- if (!is.null(dim(coef(pls_fit)))) dim(coef(pls_fit))[3] else if (!is.null(pls_fit$scores)) ncol(pls_fit$scores) else 1
idx_pls <- which.min(pls_msep$val[1, , 1])
ncomp_pls <- idx_pls - 1
ncomp_pls <- max(1, min(ncomp_pls, max_comp_pls))
pred_pls <- as.numeric(predict(pls_fit, newdata = testData, ncomp = ncomp_pls))
mse_pls <- mean((y_test - pred_pls)^2)
print(paste("PLS selected ncomp =", ncomp_pls))
print(paste("PLS test MSE =", round(mse_pls, 3)))

# (g) 汇总比较各方法的测试 MSE
print("Summary of test MSEs:")
print(sprintf("OLS:   %0.3f", mse_lm))
print(sprintf("Ridge: %0.3f (lambda=%g)", mse_ridge, signif(lambda_ridge,6)))
print(sprintf("Lasso: %0.3f (lambda=%g, nonzero=%d)", mse_lasso, signif(lambda_lasso,6), nz_count))
print(sprintf("PCR:   %0.3f (ncomp=%d)", mse_pcr, ncomp_pcr))
print(sprintf("PLS:   %0.3f (ncomp=%d)", mse_pls, ncomp_pls))