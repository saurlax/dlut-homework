disp('利用3阶泰勒公式求sin3°的近似值=');
x = 3 * pi / 180;
disp(x - x^3/factorial(3));

disp('对于x=a(t-sint), y=a(1-cost)，求dy/dx, d2y/dx2');
syms a t;
x = a * (t - sin(t));
y = a * (1 - cos(t));
dx = diff(x, t);
dy = diff(y, t);
dy_dx = dy / dx;
disp(simplify(dy_dx));
d_dy_dx = diff(dy_dx, t);
dx_dt = diff(x, t);
d2y_dx2 = d_dy_dx / dx_dt;
disp(simplify(d2y_dx2));

disp('求e^y+xy-e^x=0所确定的隐函数y=y(x)的导数dy/dx=');
syms x y;
f = exp(y) + x*y - exp(x);
fx = diff(f, x);
fy = diff(f, y);
disp(simplify(-fx/fy));
