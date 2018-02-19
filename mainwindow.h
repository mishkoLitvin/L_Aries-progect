#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QResizeEvent>
#include <QDebug>
#include <QPixmap>
#include <QImage>
#include <QSettings>
#include <QPalette>
#include <QGradient>
#include <QStyle>
#include <QInputDialog>
#include <QByteArray>

#include "headform.h"
#include "settingdialog.h"
#include "indexerwidget.h"
#include "indexersettingdialog.h"

#include "headsetting.h"

#define HEAD_COUNT 10

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    HeadForm *headButton[HEAD_COUNT];
    QImage pix;
    QSettings *settings;
    HeadSetting headSettings;
    IndexerLiftSettings indexerLiftSettings;
    SettingDialog *headSettingDialog;
    IndexerSettingDialog *indexerLiftSetDialog;
    IndexerWidget *indexer;
    QString truePassword = "qwerty";


private slots:
    void headSettingRequest(int index);
    void indexerLiftSettingRequst();
    void changeHeadNo(int index);
    void headParamGet(int index, QByteArray hParamArr);
    void allHeadParamGet(int index, QByteArray hParamArr);


protected:
   virtual void resizeEvent(QResizeEvent *e);
};

#endif // MAINWINDOW_H
