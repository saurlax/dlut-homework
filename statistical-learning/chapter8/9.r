library(ISLR); library(tree)

set.seed(1)

# (a) 划分训练/测试：随机选 800 个观测为训练，其余为测试
data(OJ)
n <- nrow(OJ)
train_idx <- sample(n, 800)
train <- OJ[train_idx, ]
test  <- OJ[-train_idx, ]

# (b) 在训练集上建立分类树（Purchase 为响应），并查看 summary，记录训练误差和终端节点数
tree.oj <- tree(Purchase ~ ., data = train)
summary(tree.oj)            # 包含误差、终端节点数量等
train.pred <- predict(tree.oj, train, type = "class")
train.err <- 1 - mean(train.pred == train$Purchase)
train.err

# (c) 打印单个终端节点的详细信息（例如第一个 terminal node）
print(tree.oj)              # 显示分支规则
# 可用 summary 或 tree.oj$frame 检视终端节点信息
tree.oj$frame[tree.oj$frame$var == "<leaf>", ]

# (d) 画树并解释
plot(tree.oj); text(tree.oj, pretty = 0)

# (e) 在测试集上预测并构建混淆矩阵，计算测试错误率
test.pred <- predict(tree.oj, test, type = "class")
table(Pred = test.pred, True = test$Purchase)
test.err <- 1 - mean(test.pred == test$Purchase)
test.err

# (f) 在训练集上用 cv.tree（分类误差）确定最优树规模
cv.oj <- cv.tree(tree.oj, FUN = prune.misclass)
cv.oj
best.size <- cv.oj$size[which.min(cv.oj$dev)]
best.size

# (g) 画出树规模 vs 交叉验证误差
plot(cv.oj$size, cv.oj$dev, type = "b", xlab = "Tree size", ylab = "CV deviance (misclass)")

# (h) 找到使交叉验证分类误差最低的树规模（见 best.size）

# (i) 对训练集按交叉验证选出的大小进行剪枝（如果无法剪到更小，则演示剪到 5 叶节点）
prune.oj <- prune.misclass(tree.oj, best = best.size)
if(is.null(prune.oj)) prune.oj <- prune.misclass(tree.oj, best = 5)
plot(prune.oj); text(prune.oj, pretty = 0)

# (j) 比较剪枝前后的训练误差
prune.train.pred <- predict(prune.oj, train, type = "class")
prune.train.err <- 1 - mean(prune.train.pred == train$Purchase)
train.err; prune.train.err

# (k) 比较剪枝前后的测试误差
prune.test.pred <- predict(prune.oj, test, type = "class")
prune.test.err <- 1 - mean(prune.test.pred == test$Purchase)
test.err; prune.test.err
