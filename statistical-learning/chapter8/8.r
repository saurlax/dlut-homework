library(ISLR); library(tree); library(randomForest); library(rpart); library(rpart.plot)

set.seed(1)

# (a) 构造二元响应并划分训练/测试集
Carseats$High <- factor(ifelse(Carseats$Sales > 8, "Yes", "No"))   # threshold 可按需调整
train_idx <- sample(seq_len(nrow(Carseats)), size = 0.7 * nrow(Carseats))
train <- Carseats[train_idx, ]
test  <- Carseats[-train_idx, ]

# (b) 构建回归树（分类树）并评估测试错误率
tree.fit <- tree(High ~ . - Sales, data = train)   # 用 High 作响应，去掉原始 Sales
plot(tree.fit); text(tree.fit, pretty=0)
tree.pred <- predict(tree.fit, newdata = test, type = "class")
(tree.cm <- table(Pred = tree.pred, Truth = test$High))
tree.err <- 1 - sum(diag(tree.cm)) / sum(tree.cm)
tree.err

# (c) 交叉验证选择最优树并剪枝，比较测试误差
cv.out <- cv.tree(tree.fit, FUN = prune.misclass)
best_size <- cv.out$size[which.min(cv.out$dev)]
prune.tree.fit <- prune.misclass(tree.fit, best = best_size)
plot(prune.tree.fit); text(prune.tree.fit, pretty=0)
prune.pred <- predict(prune.tree.fit, newdata = test, type = "class")
(prune.cm <- table(Pred = prune.pred, Truth = test$High))
prune.err <- 1 - sum(diag(prune.cm)) / sum(prune.cm)
prune.err

# (d) 装袋（bagging）并评估，查看变量重要性
# mtry = p (所有变量) 即为 bagging
p <- ncol(train) - 2   # 减去 Sales 和 High 两列
bag.fit <- randomForest(High ~ . - Sales, data = train, mtry = p, ntree = 500, importance = TRUE)
bag.pred <- predict(bag.fit, newdata = test)
(bag.cm <- table(Pred = bag.pred, Truth = test$High))
bag.err <- 1 - sum(diag(bag.cm)) / sum(bag.cm)
bag.err
importance(bag.fit)
varImpPlot(bag.fit)

# (e) 随机森林并比较，输出重要性并观察 m 对测试误差的影响（可循环 mtry）
rf.fit <- randomForest(High ~ . - Sales, data = train, mtry = floor(sqrt(p)), ntree = 500, importance = TRUE)
rf.pred <- predict(rf.fit, newdata = test)
(rf.cm <- table(Pred = rf.pred, Truth = test$High))
rf.err <- 1 - sum(diag(rf.cm)) / sum(rf.cm)
rf.err
importance(rf.fit)
varImpPlot(rf.fit)

# 比较不同 mtry 的错误率
mtry.grid <- 1:p
errs <- sapply(mtry.grid, function(m) {
  fit <- randomForest(High ~ . - Sales, data = train, mtry = m, ntree = 300)
  pred <- predict(fit, newdata = test)
  1 - mean(pred == test$High)
})
data.frame(mtry = mtry.grid, test_error = errs)