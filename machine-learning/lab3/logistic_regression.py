import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

DATASET_FILE = 'ex2data2.txt' 
POLYNOMIAL_DEGREE = 6          # 多项式次数

plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False

# 加载数据 (x, y, label)
print(f"处理数据集: {DATASET_FILE}")
data = pd.read_csv(DATASET_FILE, header=None)
X = data.iloc[:, :2].values    # 前两列是x,y坐标
y = data.iloc[:, 2].values     # 第三列是标签

print(f"数据形状: X={X.shape}, y={y.shape}")
print(f"正例: {np.sum(y==1)}, 负例: {np.sum(y==0)}")

# 自动判断是否需要多项式特征（基于数据集文件名）
USE_POLYNOMIAL = 'ex2data2' in DATASET_FILE
print(f"使用{'多项式' if USE_POLYNOMIAL else '线性'}逻辑回归")

# 特征工程
if USE_POLYNOMIAL:
    # 创建多项式特征
    X_features = np.ones((len(X), 1))
    for i in range(1, POLYNOMIAL_DEGREE + 1):
        for j in range(i + 1):
            X_features = np.column_stack([X_features, (X[:, 0] ** (i-j)) * (X[:, 1] ** j)])
    
    # 标准化（跳过bias列）
    mean = X_features[:, 1:].mean(axis=0)
    std = X_features[:, 1:].std(axis=0)
    X_norm = np.column_stack([X_features[:, 0], (X_features[:, 1:] - mean) / std])
    lr = 0.1
else:
    # 线性特征 + 标准化
    mean = X.mean(axis=0)
    std = X.std(axis=0)
    X_norm = (X - mean) / std
    lr = 0.01

# 梯度下降训练
w = np.zeros(X_norm.shape[1])
b = 0
epochs = 1000

for i in range(epochs):
    z = X_norm @ w + b
    h = 1 / (1 + np.exp(-np.clip(z, -250, 250)))  # sigmoid
    cost = -np.mean(y * np.log(h + 1e-15) + (1-y) * np.log(1-h + 1e-15))
    
    dw = X_norm.T @ (h - y) / len(y)
    db = (h - y).mean()
    w -= lr * dw
    b -= lr * db
    
    if i % 200 == 0:
        print(f"迭代 {i}, 代价: {cost:.6f}")

# 预测和评估
h_pred = 1 / (1 + np.exp(-(X_norm @ w + b)))
accuracy = np.mean((h_pred >= 0.5) == y)
print(f"准确率: {accuracy:.4f}")

# 可视化
plt.figure()
pos, neg = y == 1, y == 0
plt.scatter(X[pos, 0], X[pos, 1], c='red', marker='+', s=80, label='正例 (y=1)')
plt.scatter(X[neg, 0], X[neg, 1], c='blue', marker='o', s=60, label='负例 (y=0)')

# 绘制决策边界
x_min, x_max = X[:, 0].min() - 0.5, X[:, 0].max() + 0.5
y_min, y_max = X[:, 1].min() - 0.5, X[:, 1].max() + 0.5
xx, yy = np.meshgrid(np.linspace(x_min, x_max, 200), np.linspace(y_min, y_max, 200))
grid = np.c_[xx.ravel(), yy.ravel()]

if USE_POLYNOMIAL:
    # 多项式特征变换
    grid_poly = np.ones((len(grid), 1))
    for i in range(1, POLYNOMIAL_DEGREE + 1):
        for j in range(i + 1):
            grid_poly = np.column_stack([grid_poly, (grid[:, 0] ** (i-j)) * (grid[:, 1] ** j)])
    grid_norm = np.column_stack([grid_poly[:, 0], (grid_poly[:, 1:] - mean) / std])
else:
    # 线性特征变换
    grid_norm = (grid - mean) / std

Z = 1 / (1 + np.exp(-(grid_norm @ w + b)))
Z = Z.reshape(xx.shape)

# 绘制决策边界和概率等高线
plt.contour(xx, yy, Z, levels=[0.5], colors='green', linestyles='--', linewidths=3, label='决策边界')
plt.contourf(xx, yy, Z, levels=50, alpha=0.3)

plt.xlabel('x1')
plt.ylabel('x2')
plt.title(f'{DATASET_FILE} - {"多项式" if USE_POLYNOMIAL else "线性"}逻辑回归 (准确率: {accuracy:.3f})')
plt.legend()
plt.grid(True, alpha=0.3)
plt.colorbar(label='预测概率')
plt.show()
