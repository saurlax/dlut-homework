set.seed(1)
library(e1071)

# 生成数据：100 个观测，两个特征，非线性边界 (环形分布)
n <- 100
r <- sqrt(runif(n, 0, 4))               # 半径
theta <- runif(n, 0, 2*pi)
x1 <- r * cos(theta)
x2 <- r * sin(theta)
# 人为制造两类：内圈为 Class1，外圈为 Class2（非线性）
y <- factor(ifelse(r < 1.2, "A", "B"))
dat <- data.frame(x1 = x1, x2 = x2, y = y)

# 划分训练/测试
train_idx <- sample(1:n, 70)
train <- dat[train_idx, ]
test  <- dat[-train_idx, ]

# 训练 SVM：多项式核 (degree = 3) 与 径向基核
svm.poly <- svm(y ~ ., data = train, kernel = "polynomial", degree = 3, cost = 1, scale = TRUE)
svm.rbf  <- svm(y ~ ., data = train, kernel = "radial", gamma = 1, cost = 1, scale = TRUE)

# 在网格上绘制决策边界与训练点/测试点
plot_svm <- function(model, data, title){
  xrange <- seq(min(dat$x1)-0.5, max(dat$x1)+0.5, length=200)
  yrange <- seq(min(dat$x2)-0.5, max(dat$x2)+0.5, length=200)
  g <- expand.grid(x1 = xrange, x2 = yrange)
  preds <- predict(model, g, decision.values = FALSE)
  z <- matrix(as.numeric(preds == levels(dat$y)[1]), nrow = length(xrange), byrow = FALSE)
  plot(data$x1, data$x2, col = ifelse(data$y==levels(dat$y)[1], "red", "blue"),
       pch = 19, main = title, xlab = "x1", ylab = "x2")
  contour(xrange, yrange, z, levels = 0.5, add = TRUE, drawlabels = FALSE, lwd = 2)
}

par(mfrow = c(1,2))
plot_svm(svm.poly, train, "Polynomial kernel (train)")
plot_svm(svm.rbf,  train, "RBF kernel (train)")
par(mfrow = c(1,1))

# 计算训练与测试错误率
pred.poly.train <- predict(svm.poly, train); pred.poly.test <- predict(svm.poly, test)
pred.rbf.train  <- predict(svm.rbf,  train); pred.rbf.test  <- predict(svm.rbf,  test)

err <- function(truth, pred) mean(truth != pred)
results <- data.frame(
  method = c("poly_deg3", "rbf"),
  train_error = c(err(train$y, pred.poly.train), err(train$y, pred.rbf.train)),
  test_error  = c(err(test$y, pred.poly.test),  err(test$y, pred.rbf.test))
)
print(results)
