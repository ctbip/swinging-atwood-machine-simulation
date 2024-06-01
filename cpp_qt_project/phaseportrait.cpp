#include "phaseportrait.h"
#include "ui_phaseportrait.h"

PhasePortrait::PhasePortrait(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PhasePortrait)
{
    ui->setupUi(this);
}

PhasePortrait::~PhasePortrait()
{
    delete ui;
}
