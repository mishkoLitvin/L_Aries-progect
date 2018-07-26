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
    void setRegisters(Register *reg);
    void setIndexerSetting(bool disconnect = true);
    void setLiftSetting(bool disconnect = true);
    void setIndexerSetting(IndexerLiftSettings::IndexParameters indexParam, bool disconnect = true);
    void setLiftSetting(IndexerLiftSettings::LiftParameters liftParam, bool disconnect = true);
    void setLiftDistance(float distance, int liftGearRatio);
    float getLiftDistance();
    void setLiftGearRatio(uint32_t liftGearRatio);


signals:
    void indexerParamChanged(QByteArray indexParamArr);
    void liftParamChanged(QByteArray liftParamArr);
    void sendCommand(QByteArray command);
    void liftDistanceChanged(float distance);


private:
    Ui::IndexerSettingDialog *ui;

    bool acceptOnDeactilationEn;
    bool acceptEnable;
    uint32_t liftGearRatio;
    Register *registers;

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
    void changeEvent(QEvent *event);
};


#endif // INDEXERSETTINGDIALOG_H
