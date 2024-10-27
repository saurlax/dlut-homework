[x, y] = meshgrid(-5:0.1:5);
z = sqrt(x.^2 - y.^2);
z(x.^2 - y.^2 < 0) = NaN;
surf(x, y, z);
hold on;
surf(x, y, -z);

v = 0:pi/256:2*pi;
x = 2*cos(v);
y = 2*sin(v);
z = 3*v;
plot3(x, y, z);

[x, y] = meshgrid(-10:0.1:10);
for k = [1, 10, -10]
  z = x.^2 + y.^2 + k*x.*y;
  figure;
  surf(x, y, z);
end

[x, y] = meshgrid(-10:0.1:10);
z = x.^2 - y.^2;
surf(x, y, z);
hold on;
x = -10:0.1:10;
y = ones(size(x));
z = x.^2 - 1;
plot3(x, y, z, 'r', 'LineWidth', 2);