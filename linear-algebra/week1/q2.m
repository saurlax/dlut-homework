A = rand(5, 5);
B = rand(5, 5);
C = rand(5, 5);
D = rand(5, 5);

disp("验证|A,B;C,D|!=|A||D|-|B||C|：");
disp(norm(det([A, B; C, D]) - det(A) * det(D) + det(B) * det(C)) > 1e-10);

disp("验证若A,C均为对角矩阵，且A可逆，则|A,B;C,D|=|AD-CB|");
A = diag(rand(5, 1));
C = diag(rand(5, 1));
disp(norm(det([A, B; C, D]) - det(A * D - C * B)) < 1e-10);