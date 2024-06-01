#include "runge.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QVector>
#include <QPixmap>
#include <QDialog>
#include <cmath>

QT_CHARTS_USE_NAMESPACE

CRungeKutta::CRungeKutta(){
    mu=2.394;
    r0=1.0;
    vr0=0.0;
    th0=0.5;
    vth0=0.0;
    tmax=20.0;
    N=10000;
}
CRungeKutta::CRungeKutta(double muk, double r0k, double vr0k, double th0k, double vth0k, double tmaxk, int Nk){
    mu=muk;
    r0=r0k;
    vr0=vr0k;
    th0=th0k;
    vth0=vth0k;
    tmax=tmaxk;
    N=Nk;
}

double CRungeKutta::firstEq(double r, double th, double vth){
    double p=(r*vth*vth+9.8*(cos(th)-mu))/(1+mu);
    return p; // radial acceleration
}
double CRungeKutta::secondEq(double r, double vr, double th, double vth){
    double p=-(2*vr*vth+9.8*sin(th))/r;
    return p; // angular acceleration
}
double CRungeKutta::energy(double r, double vr, double th, double vth){
    double p=mu*vr*vr/2+(vr*vr+r*r*vth*vth)/2+9.8*r*(mu-cos(th));
    return p; // energy
}

void CRungeKutta::solve(){
    double h=tmax/(N - 1.0);
    double r,vr,th,vth,t,E;
    double k1, k2, k3, k4; // raduis
    double l1, l2, l3, l4; // radial velocity
    double q1, q2, q3, q4; // angle
    double m1, m2, m3, m4; // angular velocity

    r=r0;
    vr=vr0;
    r=r0;
    vr=vr0;
    th=th0;
    t=0.0;
    vth=vth0;
    E=energy(r,vr,th,vth);

    QFile file("/Data/data.txt");
    if(file.open(QFile::ReadWrite | QFile::Text)){
        QTextStream out(&file);
        out<<t<<'\t'<<r<<'\t'<<vr<<'\t'<<th<<'\t'<<vth<<'\t'<<E<<'\n';
        for (int i=1; i<N; i++){
            k1=h*vr;
            l1=h*firstEq(r, th, vth);
            q1=h*vth;
            m1=h*secondEq(r, vr, th, vth);

            k2=h*(vr+l1/2);
            l2=h*firstEq(r+k1/2, th+q1/2, vth+m1/2);
            q2=h*(vth+m1/2);
            m2=h*secondEq(r+k1/2, vr+l1/2, th+q1/2, vth+m1/2);

            k3=h*(vr+l2/2);
            l3=h*firstEq(r+k2/2, th+q2/2, vth+m2/2);
            q3=h*(vth+m2/2);
            m3=h*secondEq(r+k2/2, vr+l2/2, th+q2/2, vth+m2/2);

            k4=h*(vr+l3);
            l4=h*firstEq(r+k3, th+q3, vth+m3);
            q4=h*(vth+m3);
            m4=h*secondEq(r+k3, vr+l3, th+q3, vth+m3);

            r+=(k1+2*k2+2*k3+k4)/6;
            vr+=(l1+2*l2+2*l3+l4)/6;
            th+=(q1+2*q2+2*q3+q4)/6;
            vth+=(m1+2*m2+2*m3+m4)/6;
            t+=h;
            E=energy(r,vr,th,vth);

            out<<t<<'\t'<<r<<'\t'<<vr<<'\t'<<th<<'\t'<<vth<<'\t'<<E<<'\n';
        }
        file.close();
    }
}
