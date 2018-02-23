#ifndef GENERALSETTINGDIALOG_H
#define GENERALSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class GeneralSettingDialog;
}

class GeneralSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GeneralSettingDialog(QWidget *parent = 0);
    ~GeneralSettingDialog();

private:
    Ui::GeneralSettingDialog *ui;
};

#endif // GENERALSETTINGDIALOG_H
