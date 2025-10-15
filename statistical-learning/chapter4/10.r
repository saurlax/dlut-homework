library(ISLR); library(MASS); library(class)

data(Weekly)

# (a) 描述统计与图形
summary(Weekly)
par(mfrow=c(2,3))
hist(Weekly$Lag1, main="Lag1", xlab=""); hist(Weekly$Lag2, main="Lag2", xlab="")
hist(Weekly$Lag3, main="Lag3", xlab=""); hist(Weekly$Lag4, main="Lag4", xlab="")
hist(Weekly$Lag5, main="Lag5", xlab=""); hist(Weekly$Volume, main="Volume", xlab="")
par(mfrow=c(1,1))
plot(Weekly$Year, Weekly$Today, main="Today over Years", xlab="Year", ylab="Today")

# (b) 整体数据上逻辑回归（包括 5 个 lag 与 Volume）
glm.full <- glm(Direction ~ Lag1+Lag2+Lag3+Lag4+Lag5+Volume,
                data=Weekly, family=binomial)
summary(glm.full)

# (c) 混淆矩阵与整体准确率（在全部数据上，用 0.5 截断）
probs <- predict(glm.full, type="response")
pred <- ifelse(probs > 0.5, "Up", "Down")
tab_all <- table(Pred=pred, True=Weekly$Direction)
tab_all
acc_all <- mean(pred == Weekly$Direction)
acc_all

# (d) 用 1990-2008 训练，2009-2010 测试，只用 Lag2 作为预测变量
train <- Weekly$Year <= 2008
test  <- !train

glm.lag2 <- glm(Direction ~ Lag2, data=Weekly, subset=train, family=binomial)
summary(glm.lag2)
probs.test <- predict(glm.lag2, newdata=Weekly[test,], type="response")
pred.test <- ifelse(probs.test > 0.5, "Up", "Down")
tab_glm <- table(Pred=pred.test, True=Weekly$Direction[test])
tab_glm
acc_glm_test <- mean(pred.test == Weekly$Direction[test])
acc_glm_test

# (e) LDA（重复 d 中过程）
lda.lag2 <- lda(Direction ~ Lag2, data=Weekly, subset=train)
pred.lda <- predict(lda.lag2, newdata=Weekly[test,])$class
tab_lda <- table(Pred=pred.lda, True=Weekly$Direction[test])
tab_lda
acc_lda_test <- mean(pred.lda == Weekly$Direction[test])
acc_lda_test

# (f) QDA（重复 d 中过程）
qda.lag2 <- qda(Direction ~ Lag2, data=Weekly, subset=train)
pred.qda <- predict(qda.lag2, newdata=Weekly[test,])$class
tab_qda <- table(Pred=pred.qda, True=Weekly$Direction[test])
tab_qda
acc_qda_test <- mean(pred.qda == Weekly$Direction[test])
acc_qda_test

# (g) K=1 的 KNN（重复 d 中过程）
# KNN 需要数值矩阵并做同一缩放（只用 Lag2）
train.X <- as.matrix(Weekly[train, "Lag2", drop=FALSE])
test.X  <- as.matrix(Weekly[test,  "Lag2", drop=FALSE])
# 标准化（在训练集上）
m <- colMeans(train.X); s <- apply(train.X,2,sd)
train.Xs <- scale(train.X, center=m, scale=s)
test.Xs  <- scale(test.X,  center=m, scale=s)
knn.pred1 <- knn(train.Xs, test.Xs, cl=Weekly$Direction[train], k=1)
tab_knn1 <- table(Pred=knn.pred1, True=Weekly$Direction[test])
tab_knn1
acc_knn1 <- mean(knn.pred1 == Weekly$Direction[test])
acc_knn1

# (h) 比较并打印各方法在测试集上的准确率
cat("Test accuracies (2009-2010):\n")
cat(sprintf("Logistic (Lag2): %.3f\n", acc_glm_test))
cat(sprintf("LDA (Lag2):     %.3f\n", acc_lda_test))
cat(sprintf("QDA (Lag2):     %.3f\n", acc_qda_test))
cat(sprintf("KNN (k=1, Lag2):%.3f\n", acc_knn1))

# (i) 变量组合/变换/交互与 K 值选择（简短自动搜索示例）
features_list <- list(
  c("Lag1","Lag2"),
  c("Lag1","Lag2","Lag3"),
  c("Lag1","Lag2","Lag3","Lag4","Lag5"),
  c("Lag1","Lag2","Volume")
)
results <- data.frame(method=character(), feat=character(), acc=numeric(), stringsAsFactors=FALSE)

# 逻辑回归与 KNN(k 搜索)
for(feat in features_list){
  fmla <- as.formula(paste("Direction ~", paste(feat, collapse="+")))
  # Logistic trained on train, tested on test
  g <- glm(fmla, data=Weekly, subset=train, family=binomial)
  p <- predict(g, newdata=Weekly[test,], type="response")
  pr <- ifelse(p>0.5,"Up","Down")
  results <- rbind(results, data.frame(method="Logistic", feat=paste(feat,collapse=","), acc=mean(pr==Weekly$Direction[test])))
  # KNN: try k in 1..11
  train.X <- scale(as.matrix(Weekly[train, feat]))
  test.X  <- scale(as.matrix(Weekly[test, feat]), center=attr(train.X,"scaled:center"), scale=attr(train.X,"scaled:scale"))
  bestk <- 1; bestacc <- 0
  for(k in seq(1,11,2)){
    kp <- knn(train.X, test.X, cl=Weekly$Direction[train], k=k)
    a <- mean(kp == Weekly$Direction[test])
    if(a > bestacc){ bestacc <- a; bestk <- k }
  }
  results <- rbind(results, data.frame(method=paste0("KNN(k=",bestk,")"), feat=paste(feat,collapse=","), acc=bestacc))
}
print(results)