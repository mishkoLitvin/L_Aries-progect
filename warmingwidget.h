#ifndef WARMINGWIDGET_H
#define WARMINGWIDGET_H

#include <QWidget>
#include <QByteArray>
#include <QShowEvent>

#include "settings.h"
#include "crc16.h"

namespace Ui {
class WarmingWidget;
}

class WarmingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WarmingWidget(QWidget *parent = 0);
    ~WarmingWidget();

    void setIconFolder(QString path);
    void setRegisters(Register *reg);

signals:
    void sendCommand(QByteArray);

private slots:
    void on_toolButton_clicked();
    void on_dSpinBoxTime_valueChanged(double arg1);
    void on_dSpinBoxCycles_valueChanged(double arg1);
    void on_dSpinBoxTemperature_valueChanged(double arg1);

private:
    Ui::WarmingWidget *ui;

    QString pathIcon;
    Register *registers;

protected:
    void showEvent(QShowEvent *ev);
};

#endif // WARMINGWIDGET_H
