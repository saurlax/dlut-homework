A = rand(4);

disp("取出A的前3行构成矩阵B");
B = A(1:3, :);
disp(B);

disp("取出A的前2列构成矩阵C");
C = A(:, 1:2);
disp(C);

disp("右下角3*2矩阵构成矩阵D");
D = A(2:4, 3:4);
disp(D);

disp("B与C的乘积构成矩阵E");
E = B * C;
disp(E);