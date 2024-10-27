syms x n;

disp('lim x->0 sin(sin(x))/x-1=');
disp(limit(sin(sin(x))/x-1, x, 0));

disp('lim n->inf tan^n(pi/4+1/n)=');
disp(limit(tan(pi/4+1/n)^n, n, inf));

disp('lim x->1- 1/(1+e^(1/x-1))=');
disp(limit(1/(1+exp(1/x-1)), x, 1, 'left'));

disp('lim x->1+ 1/(1+e^(1/x-1))=');
disp(limit(1/(1+exp(1/x-1)), x, 1, 'right'));