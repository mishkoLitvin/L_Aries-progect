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

    void setIconFolder(QString path);
    void setIndicatorState(u_int16_t state);

    void setErrorText(MachineSettings::MachineParameters machineParameters, uint16_t val);


private:
    Ui::InfoWidget *ui;
    QImage imageHome, imageLock, imageUp, imageArrows, imageEmerg, imageWarning, imageStopHand;
    QString pathIcon;
    QTime lastTime;

    QGraphicsOpacityEffect* effect[7];
    Register *localRegisters;
    QSettings *errMasages;



};

#endif // INFOWIDGET_H
