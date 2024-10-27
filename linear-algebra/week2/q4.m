people = [150000; 90000; 60000];
T = [0.7, 0.2, 0.1; 0.2, 0.7, 0.1; 0.1, 0.1, 0.8];

disp("一年后的人员分布：");
A = T * people;
disp(T * people);

disp("二年后的人员分布：");
B = T * A;
disp(B);

years = 10;
C = people;
for i = 1:years
  C = T * C;
end
disp("若干年后的人员分布：");
disp(C);