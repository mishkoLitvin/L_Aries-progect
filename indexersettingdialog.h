#ifndef INDEXERSETTINGDIALOG_H
#define INDEXERSETTINGDIALOG_H

#include <QWidget>
#include <QByteArray>
#include <QEvent>
#include <QShowEvent>
#include <QLineEdit>

#include "settings.h"
#include "numpaddialog.h"

#include "crc16.h"

namespace Ui {
class IndexerSettingDialog;
}

class IndexerSettingDialog : public QWidget
{
    Q_OBJECT

public:
    explicit IndexerSettingDialog(QWidget *parent = 0);
    ~IndexerSettingDialog();
    void setIndexerSetting(IndexerLiftSettings::IndexParameters indexParam);
    void setLiftSetting(IndexerLiftSettings::LiftParameters liftParam);

signals:
    void indexerParamChanged(QByteArray indexParamArr);
    void liftParamChanged(QByteArray liftParamArr);
    void sendCommand(QByteArray command);


private:
    Ui::IndexerSettingDialog *ui;

    bool acceptOnDeactilationEn;
    bool acceptEnable;

private slots:

    void connectAll();
    void disconnectAll();
    void accept();
    void reject();
    void eventFilterSetup();

    void dSpinBoxIndexDistance_valueChanged(double arg1);
    void spinBoxIndexHomeOffset_valueChanged(double arg1);
    void spinBoxIndexDistanceOffcet_valueChanged(double arg1);
    void spinBoxIndexSpeed_valueChanged(double arg1);
    void dSpinBoxIndexAccel_valueChanged(double arg1);
    void spinBoxindexSpeedRet_valueChanged(double arg1);
    void dSpinBoxIndexAccelRet_valueChanged(double arg1);
    void dSpinBoxLiftDownDelay_valueChanged(double arg1);
    void dSpinBoxLiftUpDelay_valueChanged(double arg1);
    void dSpinBoxLiftDistance_valueChanged(double arg1);
    void spinBoxLiftHomeOffset_valueChanged(double arg1);
    void spinBoxLiftSpeed_valueChanged(double arg1);
    void dSpinBoxLiftAccel_valueChanged(double arg1);

    void on_pButtonLiftMove_clicked();

    void on_pButtonLiftHome_clicked();

    void on_pButtonIndexMove_clicked();

    void on_pButtonIndexHome_clicked();

protected:
    bool event(QEvent *e);
    bool eventFilter(QObject *watched, QEvent *event);
    void showEvent(QShowEvent *ev);
};


#endif // INDEXERSETTINGDIALOG_H
