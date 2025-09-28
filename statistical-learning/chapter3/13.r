# 13.(a)
set.seed(1)
x = rnorm(100) # 创建包含100个服从N(0,1)分布的观测

# 13.(b)
eps = rnorm(100, 0, sqrt(0.25)) # 创建包含100个服从N(0,0.25)分布的观测

# 13.(c)
y = -1 + 0.5 * x + eps # β0=-1，β1=0.5
length(y) # 长度100

# 13.(d)
par(mfrow = c(1, 1))
plot(x, y)
## 可以看出y与x成线性正相关的关系

# 13.(e)
lm_y_x = lm(y ~ x)
summary(lm_y_x)
## β0^=-1.00942 β1^=0.49973 与 β0=-1，β1=0.5 几乎相等

# 13.(f)
par(mfrow = c(1, 1))
plot(x, y)
abline(-1, 0.5, lwd = 5, col = "green") # 在图中添加真实的直线，截距为-1，斜率为0.5，线宽为5，颜色为绿色
abline(lm_y_x, lwd = 3, col = "red") # 在图中添加回归直线，使用 lm_y_x 模型，线宽为3，颜色为红色
legend("topright", legend = c("Real", "Regression"), col = c("green", "red"), lwd = 3) # 添加图例，位置在右上角，说明绿色线代表“真实”关系，红色线代表“回归”关系，线宽均为3

# 13.(g)
lm_x2_y = lm(y ~ x + I(x ^ 2)) # 拟合包含 x 和 x^2 的线性模型
summary(lm_x2_y)
par(mfrow = c(1, 1))
plot(x, y)
abline(-1, 0.5, lwd = 5, col = "green")
curve(coef(lm_x2_y)[1] + coef(lm_x2_y)[2] * x + coef(lm_x2_y)[3] * x ^ 2, from = min(x), to = max(x), add = TRUE, col = "red", lwd = 3)
legend("topright", legend = c("Real", "Regression"), col = c("green", "red"), lwd = 3)
## 尽管多项式回归模型的 R-squared 和 Adjusted R-squared 略高，但提升并不显著。二次项 I(x^2) 的 Estimate 为 -0.02973，p 值为 0.164，明显大于 0.05，这表明二次项在统计上不显著。

# 13.(h)
set.seed(1)
x2 = rnorm(100)
eps2 = rnorm(100, 0, 0.1)
y2 = -1 + 0.5 * x2 + eps2
lm_y_x_2 = lm(y2 ~ x2)
summary(lm_y_x_2)
par(mfrow = c(1, 1))
plot(x2, y2)
abline(-1, 0.5, lwd = 5, col = "green")
abline(lm_y_x_2, lwd = 3, col = "red")
legend("topright", legend = c("Real", "Regression"), col = c("green", "red"), lwd = 3)
## 从R方值的增大和图像可以看出拟合效果更好了

# 13.(i)
set.seed(1)
x3 = rnorm(100)
eps3 = rnorm(100, 0, 0.5)
y3 = -1 + 0.5 * x2 + eps3
lm_y_x_3 = lm(y3 ~ x3)
summary(lm_y_x_3)
par(mfrow = c(1, 1))
plot(x3, y3)
abline(-1, 0.5, lwd = 5, col = "green")
abline(lm_y_x_3, lwd = 3, col = "red")
legend("topright", legend = c("Real", "Regression"), col = c("green", "red"), lwd = 3)
## 从R方值的减小和图像可以看出拟合效果更差了

# 13.(j)
confint(lm_y_x)
confint(lm_y_x_2) # 噪声降低，置信区间更小
confint(lm_y_x_3) # 噪声提高，置信区间更大
