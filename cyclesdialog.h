#ifndef CYCLESDIALOG_H
#define CYCLESDIALOG_H

#include <QDialog>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QShowEvent>
#include <QFocusEvent>
#include <QEvent>
#include <QSettings>
#include <QByteArray>

#include "settings.h"
#include "numpaddialog.h"
#include "crc16.h"

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

    void setDirection(int direction);
    void setUnloadUseState(bool state);

signals:
    void sendCommand(QByteArray cndArr);

private:
    Ui::CyclesDialog *ui;
    int headCount;
    QList<QDoubleSpinBox*> spinBoxList;
    QList<QLabel*> labelList;

    int lastCycleSel;
    QList<int*> cycleValues;
    int cycleState;
    int direction;
    bool useUnloadHead;

    QSettings *cycleSettings;

    QList<uint32_t> headStateList;
    QList<uint32_t> headStrokList;

protected:
    void showEvent(QShowEvent *ev);
    bool eventFilter(QObject *watched, QEvent *event);
    void changeEvent(QEvent *event);

private slots:
    void on_pButtonOK_clicked();
    void on_pButtonON_clicked();
    void on_pButtonPrev_clicked();
    void on_pButtonNext_clicked();

    void on_pButtonC1_clicked();
    void on_pButtonC2_clicked();
    void on_pButtonC3_clicked();
    void on_pButtonC4_clicked();
    void on_pButtonC5_clicked();
    void on_pButtonC6_clicked();
    void on_pButtonC7_clicked();
    void on_pButtonC8_clicked();

    void loadValues(int* vals);
    void saveValues(void);
};

#endif // CYCLESDIALOG_H
