syms x y;
disp(4*int(int(x^2+y^2,y,0,1-x),x,0,1));

syms u v w;
disp(int(int(int(w^2*sin(v),w,1,2),v,0,pi/2),u,0,2*pi));