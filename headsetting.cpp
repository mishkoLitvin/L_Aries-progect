#include "headsetting.h"
#include <QDebug>

void HeadSetting::fromByteArray(QByteArray hParamArr)
{
    switch(hParamArr[0])
    {
    case 0:
    {
        this->headParameters.headType = PrintHead;
        this->headParameters.powerOn = (bool)(hParamArr[1]&0x01);
        this->headParameters.speedRear = ((0x00FF&((uint16_t)hParamArr[3]))<<8)|(0x00FF&((uint16_t)hParamArr[2]));
        this->headParameters.speedFront = ((0x00FF&((uint16_t)hParamArr[5]))<<8)|(0x00FF&((uint16_t)hParamArr[4]));
        this->headParameters.limitFront = ((0x00FF&((uint16_t)hParamArr[7]))<<8)|(0x00FF&((uint16_t)hParamArr[6]));
        this->headParameters.limitRear = ((0x00FF&((uint16_t)hParamArr[9]))<<8)|(0x00FF&((uint16_t)hParamArr[8]));
        this->headParameters.stroksCount = ((0x00FF&((uint16_t)hParamArr[11]))<<8)|(0x00FF&((uint16_t)hParamArr[10]));
        break;
    }
    case 1:
    {
        this->headParameters.headType = QuartzHead;
        this->headParameters.powerOn = (bool)(hParamArr[1]&0x01);
        this->headParameters.heatTime1 = ((0x00FF&((uint16_t)hParamArr[3]))<<8)|(0x00FF&((uint16_t)hParamArr[2]));
        this->headParameters.heatTime2 = ((0x00FF&((uint16_t)hParamArr[5]))<<8)|(0x00FF&((uint16_t)hParamArr[4]));
        this->headParameters.heatPower = ((0x00FF&((uint16_t)hParamArr[7]))<<8)|(0x00FF&((uint16_t)hParamArr[2]));
        break;
    }
    case 2:
    {
        this->headParameters.headType = InfraRedHead;
        this->headParameters.powerOn = (bool)(hParamArr[1]&0x01);
        this->headParameters.heatTime1 = ((0x00FF&((uint16_t)hParamArr[3]))<<8)|(0x00FF&((uint16_t)hParamArr[2]));
        this->headParameters.heatTime2 = ((0x00FF&((uint16_t)hParamArr[5]))<<8)|(0x00FF&((uint16_t)hParamArr[4]));
        this->headParameters.limitFront = ((0x00FF&((uint16_t)hParamArr[7]))<<8)|(0x00FF&((uint16_t)hParamArr[6]));
        break;
    }
    }
}

HeadSetting::HeadParameters HeadSetting::operator =(HeadSetting::HeadParameters hParam)
{
    HeadParameters hp;
    hp.headType = hParam.headType;
    hp.powerOn = hParam.powerOn;
    hp.heatPower = hParam.heatPower;
    hp.heatTime1 = hParam.heatTime1;
    hp.heatTime2 = hParam.heatTime2;
    hp.limitFront = hParam.limitFront;
    hp.limitRear = hParam.limitRear;
    hp.speedFront = hParam.speedFront;
    hp.speedRear = hParam.speedRear;
    hp.stroksCount = hParam.stroksCount;
    return hp;
}

QByteArray HeadSetting::HeadParameters_::toByteArray()
{
    QByteArray bArr;
    bArr.resize(12);
    bArr[0] = this->headType;
    bArr[1] = (char)(this->powerOn&0x01);
    switch(this->headType)
    {
    case PrintHead:
    {
        bArr[2] = (char)(this->speedRear&0x00FF);
        bArr[3] = (char)(((this->speedRear&0xFF00)>>8)&0x00FF);
        bArr[4] = (char)(this->speedFront&0x00FF);
        bArr[5] = (char)(((this->speedFront&0xFF00)>>8)&0x00FF);
        bArr[6] = (char)(this->limitRear&0x00FF);
        bArr[7] = (char)(((this->limitRear&0xFF00)>>8)&0x00FF);
        bArr[8] = (char)(this->limitFront&0x00FF);
        bArr[9] = (char)(((this->limitFront&0xFF00)>>8)&0x00FF);
        bArr[10] = (char)(this->stroksCount&0x00FF);
        bArr[11] = (char)(((this->stroksCount&0xFF00)>>8)&0x00FF);
        break;
    }
    case QuartzHead:
    {
        bArr[2] = (char)(this->heatTime1&0x00FF);
        bArr[3] = (char)(((this->heatTime1&0xFF00)>>8)&0x00FF);
        bArr[4] = (char)(this->heatTime2&0x00FF);
        bArr[5] = (char)(((this->heatTime2&0xFF00)>>8)&0x00FF);
        bArr[6] = (char)(this->heatPower&0x00FF);
        bArr[7] = (char)(((this->heatPower&0xFF00)>>8)&0x00FF);
        bArr[8] = (char)0x55;
        bArr[9] = (char)0x55;
        bArr[10] = (char)0x55;
        bArr[11] = (char)0x55;
        break;
    }
    case InfraRedHead:
    {
        bArr[2] = (char)(this->heatTime1&0x00FF);
        bArr[3] = (char)(((this->heatTime1&0xFF00)>>8)&0x00FF);
        bArr[4] = (char)(this->heatTime2&0x00FF);
        bArr[5] = (char)(((this->heatTime2&0xFF00)>>8)&0x00FF);
        bArr[6] = (char)(this->limitFront&0x00FF);
        bArr[7] = (char)(((this->limitFront&0xFF00)>>8)&0x00FF);
        bArr[8] = (char)0x55;
        bArr[9] = (char)0x55;
        bArr[10] = (char)0x55;
        bArr[11] = (char)0x55;
        break;
    }
    }


    return bArr;
}

HeadSetting::HeadSetting(HeadParameters hParam)
{

    this->headParameters.headType = hParam.headType;
    this->headParameters.powerOn = hParam.powerOn;
    this->headParameters.speedRear = hParam.speedRear;
    this->headParameters.speedFront = hParam.speedFront;
    this->headParameters.heatPower = hParam.heatPower;
    this->headParameters.heatTime1 = hParam.heatTime1;
    this->headParameters.heatTime2 = hParam.heatTime2;
    this->headParameters.limitFront = hParam.limitFront;
    this->headParameters.limitRear = hParam.limitRear;
    this->headParameters.stroksCount = hParam.stroksCount;
}

HeadSetting::HeadSetting()
{


    HeadParameters hParam;
    this->headParameters.headType = hParam.headType;
    this->headParameters.powerOn = hParam.powerOn;
    this->headParameters.speedRear = hParam.speedRear;
    this->headParameters.speedFront = hParam.speedFront;
    this->headParameters.heatPower = hParam.heatPower;
    this->headParameters.heatTime1 = hParam.heatTime1;
    this->headParameters.heatTime2 = hParam.heatTime2;
    this->headParameters.limitFront = hParam.limitFront;
    this->headParameters.limitRear = hParam.limitRear;
    this->headParameters.stroksCount = hParam.stroksCount;
}

HeadSetting::~HeadSetting()
{

}

QByteArray HeadSetting::LiftParameters_::toByteArray()
{
    QByteArray bArr;
    bArr.resize(12);
    bArr[0] = (char)(this->distance&0x00FF);
    bArr[1] = (char)(((this->distance&0xFF00)>>8)&0x00FF);
    bArr[2] = (char)(this->homeOffcet&0x00FF);
    bArr[3] = (char)(((this->homeOffcet&0xFF00)>>8)&0x00FF);
    bArr[4] = (char)(this->speed&0x00FF);
    bArr[5] = (char)(((this->speed&0xFF00)>>8)&0x00FF);
    bArr[6] = (char)(this->acceleration&0x00FF);
    bArr[7] = (char)(((this->acceleration&0xFF00)>>8)&0x00FF);
    bArr[8] = (char)(this->delayDown&0x00FF);
    bArr[9] = (char)(((this->delayDown&0xFF00)>>8)&0x00FF);
    bArr[10] = (char)(this->delayUp&0x00FF);
    bArr[11] = (char)(((this->delayUp&0xFF00)>>8)&0x00FF);

    return bArr;
}

QByteArray HeadSetting::IndexParameters_::toByteArray()
{
    QByteArray bArr;
    bArr.resize(14);
    bArr[0] = (char)(this->distance&0x00FF);
    bArr[1] = (char)(((this->distance&0xFF00)>>8)&0x00FF);
    bArr[2] = (char)(this->homeOffcet&0x00FF);
    bArr[3] = (char)(((this->homeOffcet&0xFF00)>>8)&0x00FF);
    bArr[4] = (char)(this->distOffcet&0x00FF);
    bArr[5] = (char)(((this->distOffcet&0xFF00)>>8)&0x00FF);
    bArr[6] = (char)(this->speed&0x00FF);
    bArr[7] = (char)(((this->speed&0xFF00)>>8)&0x00FF);
    bArr[8] = (char)(this->acceleration&0x00FF);
    bArr[9] = (char)(((this->acceleration&0xFF00)>>8)&0x00FF);
    bArr[10] = (char)(this->speedRet&0x00FF);
    bArr[11] = (char)(((this->speedRet&0xFF00)>>8)&0x00FF);
    bArr[12] = (char)(this->accelerationRet&0x00FF);
    bArr[13] = (char)(((this->accelerationRet&0xFF00)>>8)&0x00FF);

    return bArr;
}
