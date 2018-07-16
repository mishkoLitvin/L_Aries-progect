/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include <QDataStream>
#include <QEvent>
#include <QMetaType>
#include <QStringList>

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE

struct ComSettings {
    QString name;
    qint32 baudRate;
    QString stringBaudRate;
    QSerialPort::DataBits dataBits;
    QString stringDataBits;
    QSerialPort::Parity parity;
    QString stringParity;
    QSerialPort::StopBits stopBits;
    QString stringStopBits;
    QSerialPort::FlowControl flowControl;
    QString stringFlowControl;
    bool localEchoEnabled;

    ComSettings operator =(ComSettings nSett);

};

Q_DECLARE_METATYPE(ComSettings);

inline QDataStream& operator<<(QDataStream& out, const ComSettings& st)
{
    out << st.name;
    out << st.baudRate;
    out << st.stringBaudRate;
    out << st.stringDataBits;
    out << st.stringParity;
    out << st.stringStopBits;
    out << st.stringFlowControl;
    out << st.localEchoEnabled;
    return out;
}
inline QDataStream& operator>>(QDataStream& in, ComSettings& st)
{
    in >> st.name;
    in >> st.baudRate;
    in >> st.stringBaudRate;
    in >> st.stringDataBits;
    in >> st.stringParity;
    in >> st.stringStopBits;
    in >> st.stringFlowControl;
    in >> st.localEchoEnabled;
    return in;
}


namespace Ui {
class SerialSettingsDialog;
}

class QIntValidator;

QT_END_NAMESPACE

class SerialSettingsDialog : public QDialog
{
    Q_OBJECT

public:

    explicit SerialSettingsDialog(QWidget *parent = 0);
    explicit SerialSettingsDialog(ComSettings nSett, QWidget *parent = 0);
    ~SerialSettingsDialog();

    ComSettings settings() const;
    void setSettings(ComSettings nSett);

signals:
    void serialSettingAccepted(ComSettings comSett);

protected:
    void changeEvent(QEvent *event);

private slots:
    void showPortInfo(int idx);
    void apply();
    void reject();
    void checkCustomBaudRatePolicy(int idx);
    void checkCustomDevicePathPolicy(int idx);

    void fillPortsParameters();
    void fillPortsInfo();
    void updateSettings();

private:
    Ui::SerialSettingsDialog *ui;
    bool acceptOnDeactilationEn;
    ComSettings currentSettings;
    QIntValidator *intValidator;

};

#endif // SETTINGSDIALOG_H
