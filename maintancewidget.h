#ifndef MAINTANCEWIDGET_H
#define MAINTANCEWIDGET_H

#include <QDialog>

class MaintanceElement {
public:
    int troubleIndex;
    int troubleType;
    QString troubleName;
    QString troubleInfo;
    QString machineInfo;
    int lastCount;
    int repeatCyclesCount;

    MaintanceElement(int index = 0,
                     QString name = "trouble",
                     QString trInfo = "trouble info",
                     QString machInfo = "machine info",
                     int lastCount = 0,
                     int repeatCount = 0)
    {
        this->troubleIndex = index;
        this->troubleType = 0;
        this->troubleName = name;
        this->troubleInfo = trInfo;
        this->machineInfo = machInfo;
        this->lastCount = lastCount;
        this->repeatCyclesCount = repeatCount;
    }


};

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
class MaintanceWidget;
}

class MaintanceWidget : public QDialog
{
    Q_OBJECT

public:
    explicit MaintanceWidget(QWidget *parent = 0);
    ~MaintanceWidget();

    void clearText();
    void setElemets(QList<MaintanceElement> elemList);

signals:
    void troubleSolved(int troubleIndex);
    void maintanceClosed(QList<MaintanceElement> elemList);
    void tutorialRequest(int troubleIndex);

protected:
    void changeEvent(QEvent *event);

private:
    Ui::MaintanceWidget *ui;

    QList<MaintanceElement> maintanceList;

private slots:
    void solved();
    void closeWindow();
    void callTutorial();

};

#endif // MAINTANCEWIDGET_H
