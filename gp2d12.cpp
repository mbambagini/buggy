#include "mbed.h"

static AnalogIn gp2d12(p19);

#define k_5 12466.0
#define k_4 -23216.0
#define k_3 14974.0
#define k_2 -3585.0
#define k_1 19.0
#define k_0 96.0

double gp2d12_read() {
    double val, res;
    
    val = gp2d12.read();
    res = k_5*(val*val*val*val*val);
    res += k_4*(val*val*val*val);
    res += k_3*(val*val*val);
    res += k_2*(val*val);
    res += k_1*val;
    res += k_0;
    
    return res;
}

/*
Matlab commands:
Y = [10    15    20    25    30    35    40    50    60    70    80   90    100];
X = [0.70  0.57  0.39  0.32  0.28  0.24  0.20  0.17  0.13  0.12  0.1  0.01  0.0001];
p = polyfit(X, Y, 5);
x_test = [0:0.05:0.7];
for i=1:length(x_test)
    y_test(i) = polyval(p, x_test(i));
end
polyval(p,0.5)
%x = 0.5;
%res = p(1)*(x^4) + p(2)*(x^3) + p(3)*(x^2) + p(4)*(x^1) + p(5)*(x^0);
figure
plot(X, Y, 'b')
hold on
plot(x_test, y_test, 'r')
*/
