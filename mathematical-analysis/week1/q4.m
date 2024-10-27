syms x C;
disp(int(x^3*exp(-x^2))+C);
disp(int(1/x*sqrt(x^2+1))+C);

disp(int(x/sin(x)^2,pi/4,pi/3));
disp(int(sin(x)^4*cos(x)^2,0,pi/2));

disp(int(3-x^2-2*x,-3,1));

disp(int(pi*(x*sin(x)^2)^2,0,pi));
disp(int(2*pi*x*x*sin(x)^2,0,pi));