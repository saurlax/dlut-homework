t = 0:pi/256:2*pi;
rho = 3*cos(3*t);
polarplot(t, rho);

t = 0:pi/256:2*pi;
x = 2*(t+sin(t));
y = 2*(cos(t)-1);
plot(x, y);

x = -10:1/256:10;
y = x.^(2/3);
plot(x, y);

y = x.*sin(x);
plot(x, y);