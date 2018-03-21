#ifndef MAINTANCEDIALOG_H
#define MAINTANCEDIALOG_H

#include <QDialog>
#include <QIcon>
#include <QDebug>
#include <QSettings>

class MaintanceElement {
public:
    int troubleType;
    QString troubleName;
    QString troubleInfo;
    QString machineInfo;
    int lastCount;
    int repeatCyclesCount;

    MaintanceElement(QString name = "trouble",
                     QString trInfo = "trouble info",
                     QString machInfo = "machine info",
                     int lastCount = 0,
                     int repeatCount = 0)
    {
        this->troubleType = 0;
        this->troubleName = name;
        this->troubleInfo = trInfo;
        this->machineInfo = machInfo;
        this->lastCount = lastCount;
        this->repeatCyclesCount = repeatCount;
    }


};

Q_DECLARE_METATYPE(MaintanceElement);

inline QDataStream& operator<<(QDataStream& out, const MaintanceElement& st)
{
    out << st.troubleType;
    out << st.troubleName;
    out << st.troubleInfo;
    out << st.machineInfo;
    out << st.lastCount;
    out << st.repeatCyclesCount;

    return out;
}
inline QDataStream& operator>>(QDataStream& in, MaintanceElement& st)
{
    in >> st.troubleType;
    in >> st.troubleName;
    in >> st.troubleInfo;
    in >> st.machineInfo;
    in >> st.lastCount;
    in >> st.repeatCyclesCount;

    return in;
}


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

signals:
    void stopRequest();
    void continueRequest();
    void maintanceWorkEnable();

private:
    Ui::MaintanceDialog *ui;

    bool doItNow;
    QSettings* settings;
    QList <MaintanceElement> maintanceList;

private slots:
    void acceptSlot();
    void rejectSlot();
};

#endif // MAINTANCEDIALOG_H
