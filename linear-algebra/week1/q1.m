A = rand(5, 5);
B = rand(5, 5);
C = rand(5, 5);
b = rand(5, 1);

disp("A+B=");
disp(A + B);
disp("A-B=");
disp(A - B);

disp("A*B+B*A=");
disp(A * B + B * A);

disp("Ax=b的解：");
x = A \ b;
disp(x);
disp("验证克莱姆法则：");
n = size(A, 1);
x_cramer = zeros(n, 1);
detA = det(A);
for i = 1:n
    Ai = A;
    Ai(:, i) = b;
    x_cramer(i) = det(Ai) / detA;
end
disp(norm(x - x_cramer) < 1e-10);

disp("A,B的行列式，逆，秩：");
disp("det(A)=");
disp(det(A));
disp("det(B)=");
disp(det(B));
disp("inv(A)=");
disp(inv(A));
disp("inv(B)=");
disp(inv(B));
disp("rank(A)=");
disp(rank(A));

disp("A*B的行列式，逆，秩：");
disp("det(A*B)=");
disp(det(A * B));
disp("inv(A*B)=");
disp(inv(A * B));
disp("rank(A*B)=");
disp(rank(A * B));

disp("验证det(A*B)=det(A)*det(B)：");
disp(norm(det(A * B) - det(A) * det(B)) < 1e-10);

disp("验证(AB)^T=B^T*A^T：");
disp(norm((A * B)' - B' * A') < 1e-10);

disp("验证(AB)^-1=B^-1*A^-1：");
disp(norm(inv(A * B) - inv(B) * inv(A)) < 1e-10);

disp("验证AB!=BA：");
disp(norm(A * B - B * A) > 1e-10);

disp("求矩阵X，使得AXB=C：");
disp(A \ C / B);