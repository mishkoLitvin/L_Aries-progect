#ifndef INDEXERWIDGET_H
#define INDEXERWIDGET_H

#include <QWidget>

#include <QResizeEvent>
#include <QPushButton>
#include <QDebug>

#include "settings.h"
#include "crc16.h"


namespace Ui {
class IndexerWidget;
}

class IndexerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IndexerWidget(QWidget *parent = 0);
    ~IndexerWidget();

    bool getIsAutoPrint();
public slots:
    void printFinish();
    void printStart();

signals:
    void settingButtonCliced();
    void sendCommand(QByteArray command);
    void startPrint(bool isAuto);
    void stopPrint();

private slots:

    void on_pButtonLock_clicked();
    void on_pButtonMove_clicked();
    void on_pButtonAuto_clicked();
    void on_pButtonPrint_clicked();
    void on_pButtonMoveLeft_clicked();
    void on_pButtonMoveUp_clicked();
    void on_pButtonMoveRight_clicked();
    void settingPButtonClicSlot();

private:
    Ui::IndexerWidget *ui;

    QPushButton *pButtonSets;

    int halfCounter;

    bool isAutoPrintEnable;

    MachineSettings::MachineState machineState;

protected:
   virtual void resizeEvent(QResizeEvent *e);

};

#endif // INDEXERWIDGET_H
