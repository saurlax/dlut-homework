A = rand(5);
b = rand(5, 1);

disp("Ax=b的解：");
disp(A \ b);

A = rand(3, 2);
disp("A*A'的秩：");
disp(rank(A * A'));

disp("A'*A的秩：");
disp(rank(A' * A));