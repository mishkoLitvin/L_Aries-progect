#ifndef INDEXERSETTINGDIALOG_H
#define INDEXERSETTINGDIALOG_H

#include <QWidget>

namespace Ui {
class indexerSettingDialog;
}

class indexerSettingDialog : public QWidget
{
    Q_OBJECT

public:
    explicit indexerSettingDialog(QWidget *parent = 0);
    ~indexerSettingDialog();

private:
    Ui::indexerSettingDialog *ui;
};

#endif // INDEXERSETTINGDIALOG_H
