#ifndef PHASEPORTRAIT_H
#define PHASEPORTRAIT_H

#include <QMainWindow>

namespace Ui {
class PhasePortrait;
}

class PhasePortrait : public QMainWindow
{
    Q_OBJECT
public:
    explicit PhasePortrait(QWidget *parent = 0);
    ~PhasePortrait();
private:
    Ui::PhasePortrait *ui;
};

#endif // PHASEPORTRAIT_H
