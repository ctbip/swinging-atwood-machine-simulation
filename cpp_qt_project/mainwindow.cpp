#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "runge.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    double t,mu,r,vr,th,vth,N,E0;
    t=ui->value_t->value();
    mu=ui->value_mu->value();
    th=ui->value_th->value()*M_PI;
    r=ui->value_r->value();
    vth=ui->value_vth->value();
    vr=ui->value_vr->value();
    N=ui->value_N->value();
    E0=mu*vr*vr/2+(vr*vr+r*r*vth*vth)/2+9.8*r*(mu-cos(th));
    CRungeKutta firstTry(mu,r,vr,th,vth,t,N);
    firstTry.solve();

    QCustomPlot graph_energy;
    QString tStr,rStr,vrStr,thStr,vthStr,eStr;
    QVector<double> tVec(N), rVec(N), vrVec(N), thVec(N), vthVec(N), eVec(N), x(N), y(N);
    QVector<QCPCurveData> dataxy(N), datappr(N), datappth(N);
    QString fileName("/Data/data.txt");
    QFile file(fileName);
    if(file.open(QFile::ReadOnly | QFile::Text)){
        QTextStream in(&file);
        for(int j=0;j<N;j++){
            in>>tStr>>rStr>>vrStr>>thStr>>vthStr>>eStr;
            tVec[j]=tStr.toDouble();
            rVec[j]=rStr.toDouble();
            vrVec[j]=vrStr.toDouble();
            thVec[j]=thStr.toDouble();
            vthVec[j]=vthStr.toDouble();
            eVec[j]=100*abs(eStr.toDouble()-E0)/E0;
            x[j]=rVec[j]*sin(thVec[j]);
            y[j]=-1*rVec[j]*cos(thVec[j]);
        }
        file.close();
    }

   ui->graph_trajectory->clearGraphs();
   ui->graph_trajectory->addGraph();
   ui->graph_trajectory->graph(0)->setData(x,y);
   ui->graph_trajectory->graph(0)->setLineStyle(QCPGraph::lsNone);
   ui->graph_trajectory->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
   ui->graph_trajectory->axisRect()->setupFullAxesBox();
   ui->graph_trajectory->rescaleAxes();
   //ui->graph_trajectory->xAxis->setRange(-1.5,1.5);
   //ui->graph_trajectory->yAxis->setRange(-1.5,0.5);

   // add the text label at the top:
    QString title;
    title = " "+QString::number(mu)+" ";
    QCPItemText *textLabel = new QCPItemText(ui->graph_trajectory);
    textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
    textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
    textLabel->position->setCoords(0.1, 0.05); // place position at center/top of axis rect
    textLabel->setText(title);
    textLabel->setFont(QFont(font().family(), 16)); // make font a bit larger
    textLabel->setPen(QPen(Qt::black)); // show black border around text
    textLabel->deleteLater();
   ui->graph_trajectory->replot();

   ui->graph_energy->clearGraphs();
   ui->graph_energy->addGraph();
   ui->graph_energy->graph(0)->setData(tVec,eVec);
   ui->graph_energy->axisRect()->setupFullAxesBox();
   ui->graph_energy->rescaleAxes();
   ui->graph_energy->replot();

   ui->graph_pp_vr->clearGraphs();
   ui->graph_pp_vr->addGraph();
   ui->graph_pp_vr->graph(0)->setData(rVec,vrVec);
   ui->graph_pp_vr->graph(0)->setLineStyle(QCPGraph::lsNone);
   ui->graph_pp_vr->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
   ui->graph_pp_vr->axisRect()->setupFullAxesBox();
   ui->graph_pp_vr->rescaleAxes();
   ui->graph_pp_vr->replot();

   ui->graph_pp_vth->clearGraphs();
   ui->graph_pp_vth->addGraph();
   ui->graph_pp_vth->graph(0)->setData(thVec,vthVec);
   ui->graph_pp_vth->graph(0)->setLineStyle(QCPGraph::lsNone);
   ui->graph_pp_vth->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
   ui->graph_pp_vth->axisRect()->setupFullAxesBox();
   ui->graph_pp_vth->rescaleAxes();
   ui->graph_pp_vth->replot();
}

void MainWindow::on_saveButton_clicked()
{
    double mu, th;

    mu = ui->value_mu->value();
    th = ui->value_th->value()*M_PI;

    QString name, fileName1, fileName2, fileName3, fileName4;

    name = QString::number(mu) + "_" +QString::number(th) + ".png";
    fileName1 = "/Pictures/Trajectory/" + name;
    fileName2 = "/Pictures/PPforR/"+name;
    fileName3 = "/Pictures/PPforTHETA/"+ name;
    fileName4 = "/Pictures/Energy/"+name;

    QFile file1(fileName1), file2(fileName2),file3(fileName3),file4(fileName4);

    if (!file1.open(QIODevice::WriteOnly))
    {
        qDebug() << file1.errorString();
    } else {
        ui->graph_trajectory->savePng(fileName1);
    }
    if (!file2.open(QIODevice::WriteOnly))
    {
        qDebug() << file2.errorString();
    } else {
        ui->graph_pp_vr->savePng(fileName2);
    }
    if (!file3.open(QIODevice::WriteOnly))
    {
        qDebug() << file3.errorString();
    } else {
        ui->graph_pp_vth->savePng(fileName3);
    }
    if (!file4.open(QIODevice::WriteOnly))
    {
        qDebug() << file4.errorString();
    } else {
        ui->graph_energy->savePng(fileName4);
    }
}

void MainWindow::on_exitButton_clicked()
{

}
