#ifndef RUNGE_H
#define RUNGE_H

#include "mainwindow.h"

class CRungeKutta{
    double mu;
    double r0, vr0, th0, vth0, tmax;
    int N;
public:
    CRungeKutta();
    CRungeKutta(double, double, double, double, double, double, int);
    double firstEq(double, double, double);
    double secondEq(double, double, double, double);
    double energy(double, double, double, double);
    void solve();
};

#endif // RUNGE_H
