#ifndef USERSETTING_H
#define USERSETTING_H

#include <QDialog>
#include <QDebug>
#include <QShowEvent>
#include <QCheckBox>
#include <QSettings>

#include "keyboarddialog.h"

namespace Ui {
class UserSettingDialog;
}

class UserSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserSettingDialog(QWidget *parent = 0);
    ~UserSettingDialog();

    bool isUser(QString userName, QString userPassw);
    QStringList getUserNames();
    bool getLoginWindowEnable();

private:
    Ui::UserSettingDialog *ui;
    int tableRowSelected;
    int tableColnumSelected;
    QSettings *usersData;

private slots:
    void tableCellActivated(int row, int col);
    void addUser();
    void removeUsers();
    void removeAllUsers();
    void acceptSlot();
    void rejectSlot();
    void loginDialogEnable();

protected:
    void showEvent(QShowEvent *ev);
    void changeEvent(QEvent *event);
};

#endif // USERSETTING_H
