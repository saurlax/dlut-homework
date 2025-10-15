# (a) Power(): 输出 2^3 并显示
Power <- function(){
  print(2^3)
}

# 演示
Power()   # 8

# (b) Power2(x,a): 输入任意 x 和 a，显示 x^a
Power2 <- function(x, a){
  print(x^a)
  invisible(x^a)
}

# 演示
Power2(3, 8)   # 6561

# (c) 用 Power2 计算 10^3, 8^17, 131^3
Power2(10, 3)
Power2(8, 17)
Power2(131, 3)

# (d) Power3(x,a): 计算 x^a 并返回 R 对象（而不是仅打印）
Power3 <- function(x, a){
  result <- x^a
  return(result)
}

# 演示
r <- Power3(3, 8)
print(r)   # 6561

# (e) 使用 Power3 绘制 f(x)=x^2 的图像，x = 1:10
x <- 1:10
y <- sapply(x, Power3, a = 2)
plot(x, y, type="b", xlab="x", ylab="x^2", main="f(x) = x^2")

# (f) PlotPower(): 绘制 x 与 x^a 的关系，a 固定，x 在指定范围
PlotPower <- function(x = 1:10, a = 3){
  y <- x^a
  plot(x, y, type="b",
       xlab = "x", ylab = paste0("x^", a),
       main = paste("Plot of x^", a, sep=""))
  invisible(data.frame(x=x, y=y))
}

# 演示
PlotPower(1:10, 3)   # x 取 1..10，a = 3