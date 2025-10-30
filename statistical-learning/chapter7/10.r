library(ISLR)   # College 数据
library(mgcv)   # gam
set.seed(1)

data(College)
df <- College

# (a) 将 Outstate 作为响应，其他作为预测变量，用逐步回归选择子集
full <- lm(Outstate ~ ., data = df)
step.model <- step(full, direction = "both", trace = 0)   # trace=0 关闭过程输出
cat("Selected model (stepwise):\n")
print(formula(step.model))
selected_terms <- attr(terms(step.model), "term.labels")
print(selected_terms)

# (b) 划分训练/测试集（70% 训练），用 GAM 在训练集上拟合。
set.seed(1)
n <- nrow(df)
train_idx <- sample(seq_len(n), size = floor(0.7 * n))
train <- df[train_idx, , drop = FALSE]
test  <- df[-train_idx, , drop = FALSE]

# 构造 GAM 公式：对被选中的数值型变量使用 s()，因子保留原样
term_for_gam <- sapply(selected_terms, function(v) {
  # 若变量是交互或函数形式，直接保留（此处简单处理常见情形）
  if(grepl(":", v) || grepl("\\(", v)) return(v)
  if(is.numeric(df[[v]])) sprintf("s(%s)", v) else v
}, USE.NAMES = FALSE)

gam_formula <- as.formula(paste("Outstate ~", paste(term_for_gam, collapse = " + ")))
cat("\nGAM formula:\n"); print(gam_formula)

# 拟合 GAM（REML）
gam_fit <- gam(gam_formula, data = train, method = "REML")
cat("\nGAM summary:\n")
print(summary(gam_fit))
plot(gam_fit, pages = 1, rug = TRUE, se = TRUE)

# (c) 在测试集上预测并评估（RMSE & R^2-like）
pred_test <- predict(gam_fit, newdata = test)
rmse <- sqrt(mean((test$Outstate - pred_test)^2))
sst <- sum((test$Outstate - mean(test$Outstate))^2)
sse <- sum((test$Outstate - pred_test)^2)
rsq_test <- 1 - sse / sst
cat(sprintf("\nTest RMSE = %.4f\nTest R^2 (pseudo) = %.4f\n", rmse, rsq_test))

# (d) 判断哪些被选中并在 GAM 中为平滑项的变量表现出非线性关系
s_tab <- summary(gam_fit)$s.table
if(!is.null(s_tab)){
  edf <- s_tab[, "edf"]            # 有效自由度
  pvals <- s_tab[, "p-value"]
  nonlinear <- rownames(s_tab)[edf > 1.1]   # edf>1 表示有非线性
  cat("\nSmooth terms summary (edf, p-value):\n")
  print(s_tab)
  if(length(nonlinear) > 0){
    cat("\n呈明显非线性关系的变量（edf>1.1）:\n")
    print(nonlinear)
  } else {
    cat("\n没有检测到显著的非线性平滑项 (edf 接近 1 或无平滑项)\n")
  }
} else {
  cat("\n模型中没有平滑项（s()），无法检测非线性\n")
}