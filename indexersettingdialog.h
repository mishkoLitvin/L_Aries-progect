#ifndef INDEXERSETTINGDIALOG_H
#define INDEXERSETTINGDIALOG_H

#include <QWidget>
#include <QByteArray>
#include <QEvent>
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

private slots:
    void accept();
    void reject();
    void eventFilterSetup();

    void on_dSpinBoxIndexDistance_valueChanged(double arg1);
    void on_spinBoxIndexHomeOffset_valueChanged(double arg1);
    void on_spinBoxIndexDistanceOffcet_valueChanged(double arg1);
    void on_spinBoxIndexSpeed_valueChanged(double arg1);
    void on_dSpinBoxIndexAccel_valueChanged(double arg1);
    void on_spinBoxindexSpeedRet_valueChanged(double arg1);
    void on_dSpinBoxIndexAccelRet_valueChanged(double arg1);
    void on_dSpinBoxLiftDownDelay_valueChanged(double arg1);
    void on_dSpinBoxLiftUpDelay_valueChanged(double arg1);
    void on_dSpinBoxLiftDistance_valueChanged(double arg1);
    void on_spinBoxLiftHomeOffset_valueChanged(double arg1);
    void on_spinBoxLiftSpeed_valueChanged(double arg1);
    void on_dSpinBoxLiftAccel_valueChanged(double arg1);

protected:
    bool event(QEvent *e);
    bool eventFilter(QObject *watched, QEvent *event);

};

#endif // INDEXERSETTINGDIALOG_H
