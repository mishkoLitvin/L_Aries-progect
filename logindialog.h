#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QEvent>

#include "numpaddialog.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

    void setUserNames(QStringList names);

    QString userName;
    QString userPassword;

private:
    Ui::LoginDialog *ui;

private slots:
    void loginAccepted();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

};

#endif // LOGINDIALOG_H
