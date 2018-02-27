#ifndef INDEXERSETTINGDIALOG_H
#define INDEXERSETTINGDIALOG_H

#include <QWidget>
#include <QByteArray>
#include <QEvent>
#include <QLineEdit>

#include "settings.h"
#include "numpaddialog.h"

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
    void setMachineSetting(MachineSettings::MachineParameters machineParam);

signals:
    void indexerParamChanged(QByteArray indexParamArr);
    void liftParamChanged(QByteArray liftParamArr);
    void machineParamChanged(QByteArray machinePararmArr);


private:
    Ui::IndexerSettingDialog *ui;

    bool acceptOnDeactilationEn;

private slots:
    void accept();
    void reject();
    void eventFilterSetup();


protected:
    bool event(QEvent *e);
    bool eventFilter(QObject *watched, QEvent *event);

};

#endif // INDEXERSETTINGDIALOG_H
