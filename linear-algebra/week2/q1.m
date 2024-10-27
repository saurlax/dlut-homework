disp("化简f(x1,x2,x3)=x1^2+3x1x2-x2^2+2x2x3+x3^2，并判断正定性：");
A = [1, 1.5, 0; 1.5, -1, 1; 0, 1, 1];
e = eig(A);
disp(e);
if all(e > 0)
  disp("正定");
elseif all(e >= 0)
  disp("半正定");
elseif all(e < 0)
  disp("负定");
else
  disp("不定");
end