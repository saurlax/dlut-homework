A = rand(5);
[d, V] = eigs(A, 3);

disp("最大的3个特征值：");
disp(d);

disp("对应的特征向量：");
disp(V);