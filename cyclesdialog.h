#ifndef CYCLESDIALOG_H
#define CYCLESDIALOG_H

#include <QDialog>
#include <QDoubleSpinBox>
#include <QList>

#include "settings.h"

#include <QDebug>

namespace Ui {
class CyclesDialog;
}

class CyclesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CyclesDialog(QWidget *parent = 0);
    explicit CyclesDialog(int headCount, QWidget *parent = 0);

    ~CyclesDialog();

private:
    Ui::CyclesDialog *ui;
    int headCount;
    QList<QDoubleSpinBox*> spinBoxList;

protected:
    void showEvent(QShowEvent *ev);

};

#endif // CYCLESDIALOG_H
