library(ISLR); library(e1071); set.seed(1)

data(Auto)
# (a) 构造二元响应：mpg > 中位数 为 1，否则为 0
Auto$mpgHigh <- factor(ifelse(Auto$mpg > median(Auto$mpg), "High", "Low"))

# 去掉原始 mpg（可保留若需）
dat <- Auto[, !(names(Auto) %in% c("mpg"))]

# 划分训练/测试（70% 训练）
n <- nrow(dat); train_idx <- sample(n, size = floor(0.7*n))
train <- dat[train_idx, ]; test <- dat[-train_idx, ]

# (b) 用 SVM（线性/径向/多项式核）并用不同 cost 值比较交叉验证错误率
costs <- c(0.01, 0.1, 1, 10)
kernels <- c("linear","radial","polynomial")
results <- data.frame()

for(k in kernels){
  for(C in costs){
    if(k == "polynomial"){
      # 使用 degree=2 的多项式核作为示例
      fit <- tune.svm(mpgHigh ~ ., data = train, kernel = "polynomial",
                      degree = 2, cost = C, tunecontrol = tune.control(cross = 5))
    } else {
      fit <- tune.svm(mpgHigh ~ ., data = train, kernel = k, cost = C,
                      tunecontrol = tune.control(cross = 5))
    }
    cv.err <- fit$best.performance
    # 在测试集上评估
    model <- fit$best.model
    pred.test <- predict(model, newdata = test)
    test.err <- mean(pred.test != test$mpgHigh)
    results <- rbind(results, data.frame(kernel = k, cost = C,
                                         cv_error = cv.err, test_error = test.err))
  }
}

print(results)

# (c) 可视化：选择表现最好的模型并画决策图（仅当使用两个预测变量时有意义）
# 这里举例用 displacement 和 horsepower 两个变量绘制最优 RBF 模型的决策边界
best <- results[which.min(results$test_error), ]
best

# 拟合最优模型（以 RBF 为例，如最优）
best.model <- svm(mpgHigh ~ displacement + horsepower, data = train,
                  kernel = ifelse(best$kernel=="polynomial","radial",best$kernel),
                  cost = best$cost, gamma = 0.01, degree = 2, scale = TRUE)

# 绘制训练点与决策边界（p=2 时有效）
plot(train$displacement, train$horsepower, col = ifelse(train$mpgHigh=="High","red","blue"),
     pch = 19, xlab = "displacement", ylab = "horsepower", main = "SVM decision boundary (train)")
# 网格预测并画等高线
xseq <- seq(min(train$displacement)-1, max(train$displacement)+1, length=200)
yseq <- seq(min(train$horsepower)-1, max(train$horsepower)+1, length=200)
g <- expand.grid(displacement = xseq, horsepower = yseq)
z <- predict(best.model, g)
zmat <- matrix(as.numeric(z=="High"), length(xseq), length(yseq))
contour(xseq, yseq, zmat, levels = 0.5, add = TRUE, drawlabels = FALSE, lwd = 2)