A = rand(10);
A = A ./ sum(A, 1);

disp("方法一");
f = poly(A);
r = roots(f);
disp("全部特征值：");
disp(r);
m = max(r);
disp("最大特征值：");
disp(m);
disp("对应的特征向量：");
disp(null(m*eye(10)-A));

disp("方法二");
[X, D] = eig(A);
disp("全部特征值：");
disp(diag(D));
[m, i] = max(diag(D));
disp("最大特征值：");
disp(m);
disp("对应的特征向量：");
disp(X(:,i));