# 8.(a)
library(ISLR)
lm_mpg_hp = lm(mpg ~ horsepower, data = Auto)
summary(lm_mpg_hp)
## i.从结果可以看出p值远小于0.05，则可以认为两者有关系
## ii.R^2的值可以看出大约60.59%的mpg变异可以用马力来解释，那么说明两者关系比较强
### iii.从系数为负数得知两者是负相关
predict(lm_mpg_hp, data.frame(horsepower = c(98))) # 预测值是24.46768
predict(lm_mpg_hp, data.frame(horsepower = c(98)), interval = "confidence") # 置信区间[23.97308, 24.96108]
# predict(lm_mpg_hp, data.frame(horsepower=c(98)), interval="confidence", level=0.90) 90%置信区间
predict(lm_mpg_hp, data.frame(horsepower = c(98)), interval = "prediction") # 预测区间[14.80940, 34.12476]

# 8.(b)
plot(Auto$horsepower, Auto$mpg) # 绘制响应变量与预测变量关系图
abline(lm_mpg_hp, col = "red") # 最小二乘回归线

# 8.(c)
par(mfrow = c(2, 2))
plot(lm_mpg_hp) # 绘制诊断图
# Residuals vs Fitted 残差与拟合值之间的关系。理想情况下，残差应该随机分布在零附近，没有明显的模式。而图中随着拟合值增大残差图呈下凸的曲线，说明二者可能是非线性关系。同时随着拟合值增大，残差范围增大说明可能存在异方差性（误差项方差非恒定）。
# Q-Q Residuals 检查残差是否符合正态分布。如果残差在图中大致沿直线分布，则表明符合正态性。
# Scale-Location 标准化残差的平方根与拟合值。
# Residuals vs Leverage 识别异常值和高杠杆值的点，图中有几个点的杠杆值比较高，残差也比较大，存在异常值。
# 综上得出mpg与horsepower两者可能是非线性关系，使用最小二乘回归拟合效果不好。
