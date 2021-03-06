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

#include "serialsettingsdialog.h"
#include "ui_serialsettingsdialog.h"
#include "generalsettingdialog.h"

#include <QtSerialPort/QSerialPortInfo>
#include <QIntValidator>
#include <QLineEdit>

QT_USE_NAMESPACE

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

SerialSettingsDialog::SerialSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialSettingsDialog)
{
    ui->setupUi(this);
    acceptOnDeactilationEn = true;

    intValidator = new QIntValidator(0, 4000000, this);

    ui->baudRateBox->setInsertPolicy(QComboBox::NoInsert);

    connect(ui->applyButton, &QPushButton::clicked,
            this, &SerialSettingsDialog::apply);
    connect(ui->pushButtonCancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(ui->serialPortInfoListBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &SerialSettingsDialog::showPortInfo);
    connect(ui->baudRateBox,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &SerialSettingsDialog::checkCustomBaudRatePolicy);
    connect(ui->serialPortInfoListBox,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &SerialSettingsDialog::checkCustomDevicePathPolicy);
    connect(ui->pButtonRescanPorts, SIGNAL(clicked(bool)), this, SLOT(fillPortsInfo()));

    fillPortsParameters();
    fillPortsInfo();

    updateSettings();
    qRegisterMetaTypeStreamOperators<ComSettings>("ComSettings");

    this->installEventFilter(this);

}

SerialSettingsDialog::SerialSettingsDialog(ComSettings nSett, QWidget *parent):
    QDialog(parent),
    ui(new Ui::SerialSettingsDialog)
{
    ui->setupUi(this);

    intValidator = new QIntValidator(0, 4000000, this);

    ui->baudRateBox->setInsertPolicy(QComboBox::NoInsert);
    this->settings() = nSett;

    connect(ui->applyButton, &QPushButton::clicked,
            this, &SerialSettingsDialog::apply);
    connect(ui->pushButtonCancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(ui->serialPortInfoListBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &SerialSettingsDialog::showPortInfo);
    connect(ui->baudRateBox,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &SerialSettingsDialog::checkCustomBaudRatePolicy);
    connect(ui->serialPortInfoListBox,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &SerialSettingsDialog::checkCustomDevicePathPolicy);
    connect(ui->pButtonRescanPorts, SIGNAL(clicked(bool)), this, SLOT(fillPortsInfo()));

    fillPortsParameters();
    fillPortsInfo();
    qRegisterMetaTypeStreamOperators<ComSettings>("ComSettings");

    int i;
    bool stay = true;
    for(i = 0; (i<ui->serialPortInfoListBox->count())&stay; i++)
    {
        if(ui->serialPortInfoListBox->itemText(i) == nSett.name)
        {
            ui->serialPortInfoListBox->setCurrentIndex(i);
            stay = false;
        }
    }
    if(!stay)
        ui->serialPortInfoListBox->addItem(nSett.name);

    stay = true;
    for(i = 0; (i<ui->baudRateBox->count())&stay; i++)
    {
        if(ui->baudRateBox->itemText(i) == nSett.stringBaudRate)
        {
            ui->baudRateBox->setCurrentIndex(i);
            stay = false;
        }
    }
    if(!stay)
        ui->baudRateBox->addItem(nSett.stringBaudRate, nSett.baudRate);

    stay = true;
    for(i = 0; (i<ui->dataBitsBox->count())&stay; i++)
        if(ui->dataBitsBox->itemText(i) == nSett.stringDataBits)
        {
            ui->dataBitsBox->setCurrentIndex(i);
            stay = false;
        }

    stay = true;
    for(i = 0; (i<ui->parityBox->count())&stay; i++)
        if(ui->parityBox->itemText(i) == nSett.stringParity)
        {
            ui->parityBox->setCurrentIndex(i);
            stay = false;
        }

    stay = true;
    for(i = 0; (i<ui->stopBitsBox->count())&stay; i++)
        if(ui->stopBitsBox->itemText(i) == nSett.stringStopBits)
        {
            ui->stopBitsBox->setCurrentIndex(i);
            stay = false;
        }
    stay = true;
    for(i = 0; (i<ui->flowControlBox->count())&stay; i++)
        if(ui->flowControlBox->itemText(i) == nSett.stringFlowControl)
        {
            ui->flowControlBox->setCurrentIndex(i);
            stay = false;
        }


    updateSettings();
}

SerialSettingsDialog::~SerialSettingsDialog()
{
    delete ui;
}

ComSettings SerialSettingsDialog::settings() const
{
    return currentSettings;
}

void SerialSettingsDialog::setSettings(ComSettings nSett)
{
    qRegisterMetaTypeStreamOperators<ComSettings>("ComSettings");

    int i;
    bool stay = true;
    for(i = 0; (i<ui->serialPortInfoListBox->count())&stay; i++)
    {
        if(ui->serialPortInfoListBox->itemText(i) == nSett.name)
        {
            ui->serialPortInfoListBox->setCurrentIndex(i);
            stay = false;
        }
    }
    if(!stay)
        ui->serialPortInfoListBox->addItem(nSett.name);

    stay = true;
    for(i = 0; (i<ui->baudRateBox->count())&stay; i++)
    {
        if(ui->baudRateBox->itemText(i) == nSett.stringBaudRate)
        {
            ui->baudRateBox->setCurrentIndex(i);
            stay = false;
        }
    }
    if(!stay)
        ui->baudRateBox->addItem(nSett.stringBaudRate, nSett.baudRate);

    stay = true;
    for(i = 0; (i<ui->dataBitsBox->count())&stay; i++)
        if(ui->dataBitsBox->itemText(i) == nSett.stringDataBits)
        {
            ui->dataBitsBox->setCurrentIndex(i);
            stay = false;
        }

    stay = true;
    for(i = 0; (i<ui->parityBox->count())&stay; i++)
        if(ui->parityBox->itemText(i) == nSett.stringParity)
        {
            ui->parityBox->setCurrentIndex(i);
            stay = false;
        }

    stay = true;
    for(i = 0; (i<ui->stopBitsBox->count())&stay; i++)
        if(ui->stopBitsBox->itemText(i) == nSett.stringStopBits)
        {
            ui->stopBitsBox->setCurrentIndex(i);
            stay = false;
        }
    stay = true;
    for(i = 0; (i<ui->flowControlBox->count())&stay; i++)
        if(ui->flowControlBox->itemText(i) == nSett.stringFlowControl)
        {
            ui->flowControlBox->setCurrentIndex(i);
            stay = false;
        }
    updateSettings();
}

void SerialSettingsDialog::showPortInfo(int idx)
{
    if (idx == -1)
        return;

    QStringList list = ui->serialPortInfoListBox->itemData(idx).toStringList();
    if((list.count() > 1)&(QString(list.at(1)).length()<15))
        ui->descriptionLabel->setText(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : tr(blankString)));
    else
        ui->descriptionLabel->setText(tr("Description: %1").arg(list.count() > 1 ? QString(QString(list.at(1)).mid(0,13)+"...") : tr(blankString)));
    ui->manufacturerLabel->setText(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : tr(blankString)));
    ui->serialNumberLabel->setText(tr("Serial number: %1").arg(list.count() > 3 ? list.at(3) : tr(blankString)));
    ui->locationLabel->setText(tr("Location: %1").arg(list.count() > 4 ? list.at(4) : tr(blankString)));
    ui->vidLabel->setText(tr("Vendor Identifier: %1").arg(list.count() > 5 ? list.at(5) : tr(blankString)));
    ui->pidLabel->setText(tr("Product Identifier: %1").arg(list.count() > 6 ? list.at(6) : tr(blankString)));
}

void SerialSettingsDialog::apply()
{
    updateSettings();
    emit this->serialSettingAccepted(this->settings());
    accept();
}

void SerialSettingsDialog::reject()
{
    accept();
}

void SerialSettingsDialog::checkCustomBaudRatePolicy(int idx)
{
    bool isCustomBaudRate = !ui->baudRateBox->itemData(idx).isValid();
    ui->baudRateBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate) {
        ui->baudRateBox->clearEditText();
        QLineEdit *edit = ui->baudRateBox->lineEdit();
        edit->setValidator(intValidator);
    }
}

void SerialSettingsDialog::checkCustomDevicePathPolicy(int idx)
{
    bool isCustomPath = !ui->serialPortInfoListBox->itemData(idx).isValid();
    ui->serialPortInfoListBox->setEditable(isCustomPath);
    if (isCustomPath)
        ui->serialPortInfoListBox->clearEditText();
}

void SerialSettingsDialog::fillPortsParameters()
{
    ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudRateBox->addItem(tr("Custom"));

    ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsBox->setCurrentIndex(3);

    ui->parityBox->addItem(tr("None"), QSerialPort::NoParity);
    ui->parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);

    ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

void SerialSettingsDialog::fillPortsInfo()
{
    ui->serialPortInfoListBox->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        ui->serialPortInfoListBox->addItem(list.first(), list);
    }

    ui->serialPortInfoListBox->addItem(tr("Custom"));
}

void SerialSettingsDialog::updateSettings()
{
    currentSettings.name = ui->serialPortInfoListBox->currentText();

    if (ui->baudRateBox->currentIndex() == 4) {
        currentSettings.baudRate = ui->baudRateBox->currentText().toInt();
    } else {
        currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                    ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
    }
    currentSettings.stringBaudRate = QString::number(currentSettings.baudRate);

    currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
                ui->dataBitsBox->itemData(ui->dataBitsBox->currentIndex()).toInt());
    currentSettings.stringDataBits = ui->dataBitsBox->currentText();

    currentSettings.parity = static_cast<QSerialPort::Parity>(
                ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt());
    currentSettings.stringParity = ui->parityBox->currentText();

    currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt());
    currentSettings.stringStopBits = ui->stopBitsBox->currentText();

    currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
                ui->flowControlBox->itemData(ui->flowControlBox->currentIndex()).toInt());
    currentSettings.stringFlowControl = ui->flowControlBox->currentText();

    currentSettings.localEchoEnabled = ui->localEchoCheckBox->isChecked();
}

void SerialSettingsDialog::changeEvent(QEvent* event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

ComSettings ComSettings::operator =(ComSettings nSett)
{
    ComSettings nStt;
    nStt.baudRate = nSett.baudRate;
    nStt.dataBits = nSett.dataBits;
    nStt.flowControl = nSett.flowControl;
    nStt.localEchoEnabled = nSett.localEchoEnabled;
    nStt.name = nSett.name;
    nStt.parity = nSett.parity;
    nStt.stopBits = nSett.stopBits;
    nStt.stringBaudRate = nSett.stringBaudRate;
    nStt.stringDataBits = nSett.stringDataBits;
    nStt.stringFlowControl = nSett.stringFlowControl;
    nStt.stringParity = nSett.stringParity;
    nStt.stringStopBits = nSett.stringStopBits;
    return nStt;
}
