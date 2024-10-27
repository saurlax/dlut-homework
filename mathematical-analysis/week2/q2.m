syms x y dx dy;
z=sqrt(x^2+y^2);
dzx=diff(z,x);
dzy=diff(z,y);
disp('dz=');
disp(dzx*dx+dzy*dy);
disp('dzxx=');
disp(diff(z,x,2));
disp('dzyy=');
disp(diff(z,y,2));
disp('dzxy=');
disp(diff(dzx,y));
disp('dzyx=');
disp(diff(dzy,x));

f = x^4 - 8*x*y + 2*y^2 - 3;
dfx = diff(f, x);
dfy = diff(f, y);
S = solve(dfx, dfy);
dfxx = diff(dfx, x);
dfyy = diff(dfy, y);
dfxy = diff(dfx, y);
for i = 1:length(S.x)
  A = double(subs(dfxx, {x, y}, {S.x(i), S.y(i)}));
  B = double(subs(dfxy, {x, y}, {S.x(i), S.y(i)}));
  C = double(subs(dfyy, {x, y}, {S.x(i), S.y(i)}));
  D = A*C - B^2;
  if D > 0
    if A > 0
      disp(['极小值 f(', num2str(double(S.x(i))), ', ', num2str(double(S.y(i))), ')=', num2str(double(subs(f, {x, y}, {S.x(i), S.y(i)})))]);
    else
      disp(['极大值 f(', num2str(double(S.x(i))), ', ', num2str(double(S.y(i))), ')=', num2str(double(subs(f, {x, y}, {S.x(i), S.y(i)})))]);
    end
  end
end