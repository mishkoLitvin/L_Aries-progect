#ifndef HEADACTIVATIONDIALOG_H
#define HEADACTIVATIONDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QList>
#include <QShowEvent>

#include <QDebug>

#include "settings.h"
#include "crc16.h"

class CheckBoxIndexed : public QCheckBox
{
    Q_OBJECT
public:
    CheckBoxIndexed(int index, QString text, QWidget* parent = 0)
    {
        this->setParent(parent);
        this->setText(text);
        this->index = index;
        connect(this, SIGNAL(clicked(bool)), this, SLOT(chClicked(bool)));
    }
    virtual ~CheckBoxIndexed() {}

signals:
    void clicked(int index);

private:
    int index;

private slots:
    void chClicked(bool)
    {
        emit this->clicked(this->index);
    }
};





namespace Ui {
class HeadActivationDialog;
}

class HeadActivationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HeadActivationDialog(int headCount, QWidget *parent = 0);
    ~HeadActivationDialog();

    void setHeadActivState(uint32_t state);
    uint32_t getHeadActivState();
    bool getHeadActivAtIndex(int index);

signals:
    void sendCommand(QByteArray cmd);
    void headStateChangedSignal(int index, bool state);

private slots:
    void pushButtonOK_clicked();
    void pushButtonCancel_clicked();
    void headStateChanged(int index);

private:
    Ui::HeadActivationDialog *ui;

    QList<CheckBoxIndexed*> checkBoxList;

    int headCount;
    uint32_t headActivState;
    bool needSendReset;

protected:
    bool event(QEvent *e);
    void showEvent(QShowEvent *ev);
    void changeEvent(QEvent *event);
};

#endif // HEADACTIVATIONDIALOG_H
