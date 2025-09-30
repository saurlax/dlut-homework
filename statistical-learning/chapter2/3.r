x <- seq(0, 1, length.out = 300)

bayes    <- rep(0.15, length(x))      # 贝叶斯误差：水平线
bias2    <- 0.90 * (1 - x)^2          # 偏差^2：递减
variance <- 0.05 + 0.75 * x^3         # 方差：递增
train    <- 0.90 - 0.85 * x^0.7       # 训练误差：递减
test     <- bayes + bias2 + variance  # 测试误差：先降后升

matplot(
  x,
  cbind(test, train, bias2, variance, bayes),
  type = "l", lwd = 2,
  col = c("red", "blue", "darkgreen", "orange", "grey50"),
  lty = c(1, 1, 1, 1, 2),
  xlab = "光滑度",
  ylab = "误差",
  main = "偏差-方差示例"
)

legend(
  "topright",
  legend = c("测试误差", "训练误差", "偏差^2", "方差", "贝叶斯误差"),
  col = c("red", "blue", "darkgreen", "orange", "grey50"),
  lty = c(1, 1, 1, 1, 2), lwd = 2, bty = "n"
)