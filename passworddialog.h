#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
class PasswordDialog;
}

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordDialog(QWidget *parent = 0);
    ~PasswordDialog();
signals:
    void passwordCorrect(int index){

    }

private:
    Ui::PasswordDialog *ui;
};

#endif // PASSWORDDIALOG_H
