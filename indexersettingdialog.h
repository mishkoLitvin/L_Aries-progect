#ifndef INDEXERSETTINGDIALOG_H
#define INDEXERSETTINGDIALOG_H

#include <QWidget>
#include <QEvent>

#include "headsetting.h"

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


private:
    Ui::IndexerSettingDialog *ui;

    void accepted();

protected:
    bool event(QEvent *e);
};

#endif // INDEXERSETTINGDIALOG_H
