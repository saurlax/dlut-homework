import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
sns.set(context="notebook", style="whitegrid", palette="dark")
df = pd.read_csv('ex1data1.txt', names=['population', 'profit'])


def get_X(df):
    """加一列常数1作为截距项，返回特征矩阵（不含最后一列标签）。"""
    ones = pd.DataFrame({'ones': np.ones(len(df))}, index=df.index)
    data = pd.concat([ones, df], axis=1)
    return data.iloc[:, :-1].to_numpy()


def get_y(df):
    """返回标签向量（最后一列）。"""
    return np.array(df.iloc[:, -1])


def normalize_feature(df):
    """对每一列特征做标准化（均值0，方差1）。"""
    return df.apply(lambda column: (column - column.mean()) / column.std())


data = pd.read_csv('ex1data1.txt', names=['population', 'profit'])

X = get_X(data)
print(X.shape, type(X))

y = get_y(data)
print(y.shape, type(y))

theta = np.zeros(X.shape[1])


def lr_cost(theta, X, y):
    """计算线性回归的均方误差损失函数。"""
    m = X.shape[0]
    inner = X @ theta - y
    square_sum = inner.T @ inner
    cost = square_sum / (2 * m)
    return cost


lr_cost(theta, X, y)


def gradient(theta, X, y):
    """计算损失函数对参数theta的梯度。"""
    m = X.shape[0]
    inner = X.T @ (X @ theta - y)
    return inner / m


def batch_gradient_descent(theta, X, y, epoch, alpha=0.01):
    """批量梯度下降，返回最终参数和每轮损失。"""
    cost_data = [lr_cost(theta, X, y)]
    _theta = theta.copy()
    for _ in range(epoch):
        _theta = _theta - alpha * gradient(_theta, X, y)
        cost_data.append(lr_cost(_theta, X, y))
    return _theta, cost_data


epoch = 500
final_theta, cost_data = batch_gradient_descent(theta, X, y, epoch)
print('final_theta:', final_theta)
print('cost_data:', cost_data)
print('final_cost:', lr_cost(final_theta, X, y))

ax = sns.lineplot(x=np.arange(epoch+1), y=cost_data)
plt.xlabel('epoch')
plt.ylabel('cost')
plt.show()

b = final_theta[0]
m = final_theta[1]
print('b (intercept):', b)
print('m (slope):', m)
plt.scatter(data.population, data.profit, label="Training data")
plt.plot(data.population, data.population*m + b, label="Prediction")
plt.legend(loc=2)
plt.show()

raw_data = pd.read_csv('ex1data2.txt', names=['square', 'bedrooms', 'price'])

data = normalize_feature(raw_data)
data.head()

X = get_X(data)
print(X.shape, type(X))

y = get_y(data)
print(y.shape, type(y))

alpha = 0.01
theta = np.zeros(X.shape[1])
epoch = 500

final_theta, cost_data = batch_gradient_descent(
    theta, X, y, epoch, alpha=alpha)
print('final_theta (multi):', final_theta)
print('cost_data (multi):', cost_data)
print('final_cost (multi):', lr_cost(final_theta, X, y))
sns.lineplot(x=np.arange(len(cost_data)), y=cost_data)
plt.xlabel('epoch', fontsize=18)
plt.ylabel('cost', fontsize=18)
plt.show()


base = np.logspace(-1, -5, num=4)
candidate = np.sort(np.concatenate((base, base*3)))
print(candidate)

epoch = 50

fig, ax = plt.subplots(figsize=(16, 9))

for alpha in candidate:
    _, cost_data = batch_gradient_descent(theta, X, y, epoch, alpha=alpha)
    ax.plot(np.arange(epoch+1), cost_data, label=alpha)

ax.set_xlabel('epoch', fontsize=18)
ax.set_ylabel('cost', fontsize=18)
ax.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.)
ax.set_title('learning rate', fontsize=18)
plt.show()


def normalEqn(X, y):
    """正规方程法，直接求解最优参数theta。"""
    theta = np.linalg.pinv(X) @ y
    return theta


final_theta2 = normalEqn(X, y)
print('final_theta2 (normalEqn):', final_theta2)
