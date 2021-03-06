#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QFrame>
#include <QImage>
#include <QBitmap>
#include <QGraphicsEffect>
#include <QDebug>
#include <QTime>
#include <QSettings>

#include "settings.h"

namespace Ui {
class InfoWidget;
}

class InfoWidget : public QFrame
{
    Q_OBJECT

public:
    explicit InfoWidget(QWidget *parent = 0);
    ~InfoWidget();

    void setRegisterPointer(Register* regPtr);

    void setPrinted(int val);
    void setTotal(int val);
    void setRemaining(int val);
    void setSkipped(int val);

    void setIconFolder(QString path);
    void setIndicatorState(uint16_t state);

    void setErrorText(MachineSettings::MachineParameters machineParameters, uint16_t val);
    void setText(QString text);


protected:
    void changeEvent(QEvent *event);

private:
    Ui::InfoWidget *ui;
    QImage imageHome, imageLock, imageUp, imageArrows, imageEmerg, imageWarning, imageStopHand;
    QString pathIcon;
    QTime lastTime;

    QGraphicsOpacityEffect* effect[7];
    Register *localRegisters;
    QSettings *errMasages;

    int remainCnt, printCnt;


};

#endif // INFOWIDGET_H
