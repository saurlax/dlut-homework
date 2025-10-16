library(ISLR); library(MASS); library(class)

# 数据准备
data(Auto)
Auto <- na.omit(Auto) # 若有缺失值则删除
Auto$mpg01 <- ifelse(Auto$mpg > median(Auto$mpg), 1, 0)
Auto$mpg01 <- factor(Auto$mpg01, levels=c(0,1)) # 因变量为因子

# (b) 快速探索：摘要、箱线图、散点矩阵
summary(Auto)
par(mfrow=c(2,3))
boxplot(horsepower ~ mpg01, data=Auto, main="horsepower vs mpg01")
boxplot(weight ~ mpg01, data=Auto, main="weight vs mpg01")
boxplot(displacement ~ mpg01, data=Auto, main="displacement vs mpg01")
boxplot(acceleration ~ mpg01, data=Auto, main="acceleration vs mpg01")
boxplot(cylinders ~ mpg01, data=Auto, main="cylinders vs mpg01")
par(mfrow=c(1,1))
pairs(~ mpg + horsepower + weight + displacement + acceleration, data=Auto,
      main="pairs plot")

# 选择用于预测的变量（可根据 (b) 结果调整）
preds <- c("horsepower","weight","displacement","acceleration","year","cylinders")

# (c) 划分训练/测试集
set.seed(1)
n <- nrow(Auto)
train.idx <- sample(seq_len(n), size = floor(0.7*n))
train <- Auto[train.idx, ]
test  <- Auto[-train.idx, ]

# (d) LDA
lda.fit <- lda(as.formula(paste("mpg01 ~", paste(preds, collapse="+"))), data=train)
lda.pred <- predict(lda.fit, newdata=test)$class
tab.lda <- table(Pred=lda.pred, True=test$mpg01)
acc.lda <- mean(lda.pred == test$mpg01)

# (e) QDA
qda.fit <- qda(as.formula(paste("mpg01 ~", paste(preds, collapse="+"))), data=train)
qda.pred <- predict(qda.fit, newdata=test)$class
tab.qda <- table(Pred=qda.pred, True=test$mpg01)
acc.qda <- mean(qda.pred == test$mpg01)

# (f) Logistic 回归
glm.fit <- glm(as.formula(paste("mpg01 ~", paste(preds, collapse="+"))),
               data=train, family=binomial)
glm.prob <- predict(glm.fit, newdata=test, type="response")
glm.pred <- factor(ifelse(glm.prob > 0.5, 1, 0), levels=c(0,1))
tab.glm <- table(Pred=glm.pred, True=test$mpg01)
acc.glm <- mean(glm.pred == test$mpg01)

# (g) KNN（尝试多个 K，找最佳）
train.X <- scale(as.matrix(train[, preds]))
test.X  <- scale(as.matrix(test[, preds]),
                 center=attr(train.X, "scaled:center"),
                 scale=attr(train.X, "scaled:scale"))
train.y <- train$mpg01

ks <- seq(1,15,2)
knn.results <- data.frame(k=ks, acc=NA)
for(i in seq_along(ks)){
  k <- ks[i]
  pred.k <- knn(train.X, test.X, cl=train.y, k=k)
  knn.results$acc[i] <- mean(pred.k == test$mpg01)
}
best.knn <- knn.results[which.max(knn.results$acc), ]

# 输出结果
cat("LDA Confusion:\n"); print(tab.lda); cat("LDA Accuracy:", acc.lda, "\n\n")
cat("QDA Confusion:\n"); print(tab.qda); cat("QDA Accuracy:", acc.qda, "\n\n")
cat("Logistic Confusion:\n"); print(tab.glm); cat("Logistic Accuracy:", acc.glm, "\n\n")
cat("KNN results (k, accuracy):\n"); print(knn.results)
cat("Best K:", best.knn$k, "Accuracy:", best.knn$acc, "\n")