#ifndef MAINTANCEDIALOG_H
#define MAINTANCEDIALOG_H

#include <QDialog>
#include <QIcon>
#include <QDebug>
#include <QSettings>

#include "maintancewidget.h"

Q_DECLARE_METATYPE(QList<int>)

namespace Ui {
class MaintanceDialog;
}

class MaintanceDialog : public QDialog
{
    Q_OBJECT

public:

    typedef enum {Warning, Critical} MaintanceType;

    explicit MaintanceDialog(QWidget *parent = 0);
    explicit MaintanceDialog(MaintanceType mType,
                             QWidget *parent = 0,
                             QString name = "trouble",
                             QString trInfo = "trouble info",
                             QString machInfo = "machine info");

    ~MaintanceDialog();

    void setTrableName(QString name = "trouble");
    void setTrableInfo(QString info = "trouble info");
    void setMachineInfo(QString info = "machine info");
    void setMaintanceType(MaintanceType mType = Warning);

    static bool execute(QWidget *parent = 0,
                        MaintanceType mType = Warning,
                        QString name = "trouble",
                        QString trInfo = "trouble info",
                        QString machInfo = "machine info");

    static bool execute(MaintanceElement maintance, QWidget *parent = 0);

    void check(int cyclesCount);

public slots:
    void openMaintanceList();

signals:
    void stopRequest();
    void continueRequest();
    void maintanceWorkEnable(bool enabled);

private:
    Ui::MaintanceDialog *ui;

    bool doItNow;
    QSettings* settings;
    QList <MaintanceElement> maintanceList;
    QList <MaintanceElement> unsolvedList;
    QList <int> unsolvedListIndex;
    MaintanceWidget* maintanceWidget;
    bool firstCheck;
    int lastCyclesCount;

private slots:
    void acceptSlot();
    void rejectSlot();
    void solveItem(int index);
};

#endif // MAINTANCEDIALOG_H
