#ifndef COUNTERSDIALOG_H
#define COUNTERSDIALOG_H

#include <QWidget>
#include <QShowEvent>

#include "numpaddialog.h"

namespace Ui {
class CountersDialog;
}

class CountersDialog : public QWidget
{
    Q_OBJECT

public:
    explicit CountersDialog(QWidget *parent = 0);
    ~CountersDialog();
    
    void setRemaining(int val);
    void hideRequest();
    
signals:
    void resetSkipped();
    void resetRemaining();
    void remainingValChanged(int val);

protected:
    bool event(QEvent *e);
    void showEvent(QShowEvent *ev);
    bool eventFilter(QObject *watched, QEvent *event);
    void changeEvent(QEvent *event);

private slots:
    void on_pushButtonRemainReset_clicked();
    void on_pushButtonSkippedReset_clicked();
    void on_pushButtonHide_clicked();
    void doubleSpinBoxRemain_valueChanged(double arg1);

private:
    Ui::CountersDialog *ui;

    bool acceptOnDeactilationEn;
};

#endif // COUNTERSDIALOG_H
