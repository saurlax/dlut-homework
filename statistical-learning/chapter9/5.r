# 生成数据并比较逻辑回归与 SVM（非线性核）
set.seed(1)
library(e1071)

# (a) 生成 n=500, p=2 的数据，二类，二次决策边界
n <- 500
x1 <- runif(n) - 0.5
x2 <- runif(n) - 0.5
y <- factor(ifelse(x1^2 - x2^2 > 0, "A", "B"))
dat <- data.frame(x1 = x1, x2 = x2, y = y)

# 划分训练/测试
train_idx <- sample(n, 350)
train <- dat[train_idx, ]
test  <- dat[-train_idx, ]

# (b) 绘图（训练集，按类别上色）
plot(train$x1, train$x2, col = ifelse(train$y=="A","red","blue"),
     pch = 19, xlab = "x1", ylab = "x2", main = "Training data")

# (c) 用逻辑回归（原始 x1, x2）拟合
glm.fit <- glm(y ~ x1 + x2, data = train, family = binomial)
glm.pred.test <- predict(glm.fit, newdata = test, type = "response")
glm.class.test <- factor(ifelse(glm.pred.test > 0.5, "A", "B"), levels=levels(y))
mean(glm.class.test != test$y)   # 测试错误率

# (d) 用逻辑回归在训练集上预测并绘制预测类别（线性决策边界应为线性）
glm.pred.train <- predict(glm.fit, newdata = train, type = "response")
glm.class.train <- factor(ifelse(glm.pred.train > 0.5, "A", "B"))
plot(train$x1, train$x2, col = ifelse(glm.class.train=="A","red","blue"),
     pch = 19, main = "GLM predicted (train)")

# (e) 使用非线性变换（x1^2, x2^2, x1*x2, log(|x2|+1)）并用逻辑回归拟合
train2 <- transform(train, x1sq = x1^2, x2sq = x2^2, x1x2 = x1*x2, lx2 = log(abs(x2)+1))
test2  <- transform(test,  x1sq = x1^2, x2sq = x2^2, x1x2 = x1*x2, lx2 = log(abs(x2)+1))
glm2.fit <- glm(y ~ x1 + x2 + x1sq + x2sq + x1x2 + lx2, data = train2, family = binomial)
glm2.pred.test <- factor(ifelse(predict(glm2.fit, newdata = test2, type="response")>0.5,"A","B"), levels=levels(y))
mean(glm2.pred.test != test$y)  # 测试错误率（应比线性好）

# (f) 用该扩展特征在训练集上绘图（显示非线性决策边界是否明显）
glm2.pred.train <- factor(ifelse(predict(glm2.fit, newdata = train2, type="response")>0.5,"A","B"))
plot(train$x1, train$x2, col = ifelse(glm2.pred.train=="A","red","blue"),
     pch = 19, main = "GLM with nonlinear features (train)")

# (g) 用支持向量机（线性核）拟合原始特征
svm.lin <- svm(y ~ x1 + x2, data = train, kernel = "linear", cost = 1)
mean(predict(svm.lin, test) != test$y)  # 线性 SVM 测试错误率

# (h) 用非线性核的 SVM（RBF）拟合并比较
svm.rbf <- svm(y ~ x1 + x2, data = train, kernel = "radial", gamma = 1, cost = 1)
mean(predict(svm.rbf, test) != test$y)  # RBF SVM 测试错误率

# (i) 结果评估：打印各方法测试错误率
cat("GLM (linear) test error:", round(mean(glm.class.test != test$y),4), "\n")
cat("GLM (nonlinear features) test error:", round(mean(glm2.pred.test != test$y),4), "\n")
cat("SVM (linear) test error:", round(mean(predict(svm.lin, test) != test$y),4), "\n")
cat("SVM (RBF) test error:", round(mean(predict(svm.rbf, test) != test$y),4), "\n")