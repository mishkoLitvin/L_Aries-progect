#ifndef MAINTANCEDIALOG_H
#define MAINTANCEDIALOG_H

#include <QDialog>

namespace Ui {
class MaintanceDialog;
}

class MaintanceDialog : public QDialog
{
    Q_OBJECT

public:

    typedef enum {Warning, Critical} MaintanceType;
    explicit MaintanceDialog(QWidget *parent = 0);
    ~MaintanceDialog();

    void setTrableName(QString name);
    void setTrableInfo(QString info);
    void setMachineInfo(QString info);

private:
    Ui::MaintanceDialog *ui;
};

#endif // MAINTANCEDIALOG_H
