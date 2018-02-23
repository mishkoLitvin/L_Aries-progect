#ifndef GENERALSETTINGDIALOG_H
#define GENERALSETTINGDIALOG_H

#include <QDialog>
#include <QDebug>

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
    bool acceptOnDeactilationEn;

private slots:
    void accept();
    void reject();
    void lockUnlockEmail();

protected:
    bool event(QEvent *e);
};

#endif // GENERALSETTINGDIALOG_H
