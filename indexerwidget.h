#ifndef INDEXERWIDGET_H
#define INDEXERWIDGET_H

#include <QWidget>

#include <QResizeEvent>
#include <QPushButton>
#include <QDebug>
#include <QThread>

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
    void setIconFolder(QString path);
    void clickButton(QByteArray data);
    void setSettingEnable(bool enable);

public slots:
    void printFinish();
    void printStart();
    void setState(u_int16_t state);
    void resetWidget();

signals:
    void settingButtonCliced();
    void sendCommand(QByteArray command);
    void resetRequest();
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
    void on_pButtonReset_clicked();
    void on_pButtonHome_clicked();
    void settingPButtonClicSlot();


    void on_pButtonAir_clicked();

private:
    Ui::IndexerWidget *ui;

    QPushButton *pButtonSets;

    int halfCounter;

    bool isAutoPrintEnable;
    QString pathIcon;

    MachineSettings::MachineState machineState;

protected:
    virtual void resizeEvent(QResizeEvent *e);
    void changeEvent(QEvent *event);
};

#endif // INDEXERWIDGET_H
