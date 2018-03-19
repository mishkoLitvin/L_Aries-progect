#ifndef USERSETTING_H
#define USERSETTING_H

#include <QDialog>
#include <QDebug>
#include <QShowEvent>
#include <QCheckBox>

namespace Ui {
class UserSetting;
}

class UserSetting : public QDialog
{
    Q_OBJECT

public:
    explicit UserSetting(QWidget *parent = 0);
    ~UserSetting();

private:
    Ui::UserSetting *ui;
    int tableRowSelected;
    int tableColnumSelected;

private slots:
    void tableCellActivated(int row, int col);
    void addUser();
    void removeUsers();
    void removeAllUsers();

protected:
    void showEvent(QShowEvent *ev);

};

#endif // USERSETTING_H
