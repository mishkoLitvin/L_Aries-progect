#ifndef INDEXERSETTINGDIALOG_H
#define INDEXERSETTINGDIALOG_H

#include <QWidget>
#include <QByteArray>
#include <QEvent>
#include <QLineEdit>

#include "headsetting.h"
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

signals:
    void indexerParamChanged(QByteArray indexParamArr);
    void liftParamChanged(QByteArray liftParamArr);

private:
    Ui::IndexerSettingDialog *ui;

    void accepted();
    void rejected();

    void eventFilterSetup();

    bool acceptOnDeactilationEn;

protected:
    bool event(QEvent *e);
    bool eventFilter(QObject *watched, QEvent *event);

};

#endif // INDEXERSETTINGDIALOG_H
