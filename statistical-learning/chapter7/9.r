library(MASS)      # Boston 数据
library(splines)   # bs()

data(Boston)
x <- Boston$dis
y <- Boston$nox
set.seed(1)

# (a) 用 poly() 拟合三次多项式并绘图、输出结果
# 为避免不同 degree 的 poly() 在 predict 时发生内部属性不匹配，先计算到最大阶数的 poly
maxdeg <- 10
pp_global <- poly(x, maxdeg)  # orthogonal polynomials up to maxdeg

# 拟合并显示三次多项式（使用 pp_global 的前 3 列）
df3 <- cbind(y = y, as.data.frame(pp_global[, 1:3]))
names(df3)[-1] <- paste0("X", 1:3)
fit3 <- lm(y ~ ., data = df3)
summary(fit3)

# 绘图：数据点 + 三次多项式拟合曲线
plot(x, y, pch=16, xlab="dis", ylab="nox", main="Cubic polynomial fit (poly(x,3))")
xs <- seq(min(x), max(x), length=200)
# 使用 predict.poly 对新数据计算相同的多项式基，再取前 3 列
ys3 <- as.data.frame(predict(pp_global, xs)[, 1:3])
names(ys3) <- paste0("X", 1:3)
ys3 <- predict(fit3, newdata = ys3)
lines(xs, ys3, col="red", lwd=2)

# (b) 尝试不同阶数（1:10），计算训练 RSS 并绘出拟合曲线
RSS <- numeric(maxdeg)
fits <- vector("list", maxdeg)
for(d in 1:maxdeg){
  # 使用全样本计算的 orthogonal polynomials 的前 d 列作为预测变量
  X <- as.data.frame(pp_global[, 1:d])
  names(X) <- paste0("X", seq_len(d))
  df <- cbind(y = y, X)
  fm <- lm(y ~ ., data = df)
  RSS[d] <- sum(resid(fm)^2)
  fits[[d]] <- fm
}
plot(1:maxdeg, RSS, type="b", xlab="degree", ylab="Training RSS", main="RSS vs polynomial degree")

# 在同一图上比较若干阶数的拟合曲线（1,3,5,9 阶）
plot(x, y, pch=16, xlab="dis", ylab="nox", main="Polynomial fits (selected degrees)")
cols <- c("blue","red","green","purple")
ds <- c(1,3,5,9)
for(i in seq_along(ds)){
  dd <- ds[i]
  # 使用全样本 poly 对象预测新数据并取前 dd 列
  newX <- as.data.frame(predict(pp_global, xs)[, 1:dd])
  names(newX) <- paste0("X", seq_len(dd))
  ypred <- predict(fits[[dd]], newdata=newX)
  lines(xs, ypred, col=cols[i], lwd=2)
}
legend("topright", legend=paste0("deg=", ds), col=cols, lwd=2)

# (c) 用 10-fold CV 选择多项式阶数（自实现 K-fold）
cv_poly <- function(x, y, degree, K=10){
  n <- length(y)
  folds <- sample(rep(1:K, length.out=n))
  mse_fold <- numeric(K)
  for(k in 1:K){
    train_idx <- which(folds != k)
    test_idx  <- which(folds == k)
  # 为该折构造折内的 orthogonal poly 并用其 predict 对测试点做转换
  pptr <- poly(x[train_idx], degree)
  Xtr <- as.data.frame(pptr)
  names(Xtr) <- paste0("X", seq_len(degree))
  train_df <- cbind(y = y[train_idx], Xtr)
  fm <- lm(y ~ ., data = train_df)

  Xte <- as.data.frame(predict(pptr, x[test_idx]))
  names(Xte) <- paste0("X", seq_len(degree))
  pred <- predict(fm, newdata = Xte)
    mse_fold[k] <- mean((y[test_idx] - pred)^2)
  }
  mean(mse_fold)
}
K <- 10
cv_mse_poly <- sapply(1:maxdeg, function(d) cv_poly(x,y,d,K=K))
plot(1:maxdeg, cv_mse_poly, type="b", xlab="degree", ylab=paste0(K,"-fold CV MSE"), main="CV MSE vs polynomial degree")
best_deg_poly <- which.min(cv_mse_poly)
cat("Polynomial: best degree by CV =", best_deg_poly, "\n")

# (d) 用 bs() 拟合自由度为 4 的回归样条，并选择结点（示例取 3 个分位点）
# 方法一：直接指定 df=4
fit_bs_df4 <- lm(y ~ bs(x, df=4))
summary(fit_bs_df4)
y_bs4 <- predict(fit_bs_df4, newdata=data.frame(x=xs))
plot(x, y, pch=16, xlab="dis", ylab="nox", main="Regression spline bs(df=4)")
lines(xs, y_bs4, col="blue", lwd=2)

# 方法二：显式指定结点（例如取 25%,50%,75% 分位点作为内部结点）
knots <- quantile(x, probs=c(0.25,0.5,0.75))
fit_bs_knots <- lm(y ~ bs(x, knots=knots, degree=3))  # cubic pieces by default degree=3
summary(fit_bs_knots)
y_bs_knots <- predict(fit_bs_knots, newdata=data.frame(x=xs))
plot(x, y, pch=16, xlab="dis", ylab="nox", main="Regression spline with specified knots")
lines(xs, y_bs_knots, col="darkgreen", lwd=2)
points(knots, rep(mean(y), length(knots)), pch=4, col="red") # 标记结点位置 (在 x 轴)

# (e) 试不同的 df（自由度）拟合 bs 并比较训练 RSS 与 CV MSE
dfs <- 3:12
RSS_bs <- numeric(length(dfs))
CV_bs <- numeric(length(dfs))
for(i in seq_along(dfs)){
  dfv <- dfs[i]
  fm <- lm(y ~ bs(x, df=dfv))
  RSS_bs[i] <- sum(resid(fm)^2)
  # CV 计算（复用上面的函数，需调用 lm 内部的 bs；predict 支持）
  cvfun <- function(xtr, ytr, xts){
    fmtr <- lm(ytr ~ bs(xtr, df=dfv))
    predict(fmtr, newdata=data.frame(x=xts))
  }
  # 用同样的 10-fold 做 CV MSE（通用实现）
  folds <- sample(rep(1:K, length.out=length(y)))
  mse_f <- numeric(K)
  for(k in 1:K){
    tr <- which(folds != k); te <- which(folds == k)
    pred <- cvfun(x[tr], y[tr], x[te])
    mse_f[k] <- mean((y[te] - pred)^2)
  }
  CV_bs[i] <- mean(mse_f)
}
par(mfrow=c(1,2))
plot(dfs, RSS_bs, type="b", xlab="df (bs)", ylab="Training RSS", main="Train RSS vs bs df")
plot(dfs, CV_bs, type="b", xlab="df (bs)", ylab=paste0(K,"-fold CV MSE"), main="CV MSE vs bs df")
par(mfrow=c(1,1))
best_df_bs <- dfs[which.min(CV_bs)]
cat("Spline (bs): best df by CV =", best_df_bs, "\n")

# (f) 使用上面的 CV 结果选择自由度：best_df_bs
best_fit_bs <- lm(y ~ bs(x, df=best_df_bs))
yp_best <- predict(best_fit_bs, newdata=data.frame(x=xs))
plot(x, y, pch=16, xlab="dis", ylab="nox", main=paste("Best bs fit (df=", best_df_bs, ")", sep=""))
lines(xs, yp_best, col="red", lwd=2)