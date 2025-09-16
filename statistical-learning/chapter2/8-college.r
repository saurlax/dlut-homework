# 题 8：College 数据集

# (a) 用 read.csv 读入数据到 R
college <- read.csv("dataset/College.csv")

# (b) 将第一列院校名设为行名；用 fix() 观察；再删除第 1 列并再次用 fix()
rownames(college) <- college[, 1]
fix(college)  # 观察：左侧 row.names 已记录院校名，第 1 列仍是原始院校名

if (!is.factor(college$Private)) college$Private <- factor(college$Private)

# (c-i) 使用 summary() 查看整个数据集的汇总统计
summary(college)

# (c-ii) 使用 pairs() 对数值型变量（取前 10 个）画散点图矩阵
num_vars <- names(college)[sapply(college, is.numeric)]
pairs(college[, num_vars[seq_len(min(10, length(num_vars)))]],
      main = "Scatterplot Matrix (first 10 numeric variables)")

# (c-iii) 画 Outstate 对 Private 的箱线图（公立/私立对比）
boxplot(Outstate ~ Private, data = college,
        col = c("lightblue", "salmon"),
        ylab = "Out-of-state tuition",
        main = "Outstate by Private")

# (c-iv) 构造新的定性变量 Elite：Top10perc > 50 定义为 “Yes”，否则 “No”
college$Elite <- factor(ifelse(college$Top10perc > 50, "Yes", "No"))
summary(college$Elite)

# (c-v) 画 Outstate 对 Elite 的箱线图（是否“精英”院校）
boxplot(Outstate ~ Elite, data = college,
        col = c("gold", "grey80"),
        ylab = "Out-of-state tuition",
        main = "Outstate by Elite")

# (c-vi) 选取若干定量变量画直方图（4 宫格）：Apps, S.F.Ratio, Expend, Grad.Rate
op <- par(mfrow = c(2, 2)); on.exit(par(op), add = TRUE)
hist(college$Apps, main = "Apps", xlab = "Number of applications",
     col = "grey80", border = "white")
hist(college$S.F.Ratio, main = "S.F.Ratio", xlab = "Student/faculty ratio",
     col = "grey80", border = "white")
hist(college$Expend, main = "Expend", xlab = "Instructional expend per student",
     col = "grey80", border = "white")
hist(college$Grad.Rate, main = "Grad.Rate", xlab = "Graduation rate",
     col = "grey80", border = "white")

# 简单检查明显异常：毕业率 > 100
which(college$Grad.Rate > 100)