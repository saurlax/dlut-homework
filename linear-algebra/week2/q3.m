A = rand(5);
B = rand(5);
C = rand(5);
D = rand(5);
M = [A, B, C, D];
disp(M);

[Q, R] = qr(M, 0);
disp("正交化后的向量：");
disp(Q);