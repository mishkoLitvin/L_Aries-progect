#ifndef LOGODIALOG_H
#define LOGODIALOG_H

#include <QDialog>
#include <QShowEvent>
#include <QTimer>
#include <QMovie>

namespace Ui {
class LogoDialog;
}

class LogoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogoDialog(QWidget *parent = 0);
    ~LogoDialog();

private:
    Ui::LogoDialog *ui;
    QTimer* timer;
    QMovie *movie;


private slots:
    void timerTimeOut();

protected:
    void showEvent(QShowEvent *ev);


};

#endif // LOGODIALOG_H
