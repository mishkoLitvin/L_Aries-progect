#include "headsetting.h"
#include <QDebug>

void HeadSetting::fromByteArray(QByteArray hParamArr)
{
    switch(hParamArr[0])
    {
    case 0:
    {
        this->headParam.headType = PrintHead;
        this->headParam.powerOn = (bool)(hParamArr[1]&0x01);
        this->headParam.speedRear = ((0x00FF&((uint16_t)hParamArr[3]))<<8)|(0x00FF&((uint16_t)hParamArr[2]));
        this->headParam.speedFront = ((0x00FF&((uint16_t)hParamArr[5]))<<8)|(0x00FF&((uint16_t)hParamArr[4]));
        this->headParam.limitFront = ((0x00FF&((uint16_t)hParamArr[7]))<<8)|(0x00FF&((uint16_t)hParamArr[6]));
        this->headParam.limitRear = ((0x00FF&((uint16_t)hParamArr[9]))<<8)|(0x00FF&((uint16_t)hParamArr[8]));
        this->headParam.stroksCount = ((0x00FF&((uint16_t)hParamArr[11]))<<8)|(0x00FF&((uint16_t)hParamArr[10]));
        break;
    }
    case 1:
    {
        this->headParam.headType = QuartzHead;
        this->headParam.powerOn = (bool)(hParamArr[1]&0x01);
        this->headParam.heatTime1 = ((0x00FF&((uint16_t)hParamArr[3]))<<8)|(0x00FF&((uint16_t)hParamArr[2]));
        this->headParam.heatTime2 = ((0x00FF&((uint16_t)hParamArr[5]))<<8)|(0x00FF&((uint16_t)hParamArr[4]));
        this->headParam.heatPower = ((0x00FF&((uint16_t)hParamArr[7]))<<8)|(0x00FF&((uint16_t)hParamArr[2]));
        break;
    }
    case 2:
    {
        this->headParam.headType = InfraRedHead;
        this->headParam.powerOn = (bool)(hParamArr[1]&0x01);
        this->headParam.heatTime1 = ((0x00FF&((uint16_t)hParamArr[3]))<<8)|(0x00FF&((uint16_t)hParamArr[2]));
        this->headParam.heatTime2 = ((0x00FF&((uint16_t)hParamArr[5]))<<8)|(0x00FF&((uint16_t)hParamArr[4]));
        this->headParam.limitFront = ((0x00FF&((uint16_t)hParamArr[7]))<<8)|(0x00FF&((uint16_t)hParamArr[6]));
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

    this->headParam.headType = hParam.headType;
    this->headParam.powerOn = hParam.powerOn;
    this->headParam.speedRear = hParam.speedRear;
    this->headParam.speedFront = hParam.speedFront;
    this->headParam.heatPower = hParam.heatPower;
    this->headParam.heatTime1 = hParam.heatTime1;
    this->headParam.heatTime2 = hParam.heatTime2;
    this->headParam.limitFront = hParam.limitFront;
    this->headParam.limitRear = hParam.limitRear;
    this->headParam.stroksCount = hParam.stroksCount;
}

HeadSetting::HeadSetting()
{
    HeadParameters hParam;
    this->headParam.headType = hParam.headType;
    this->headParam.powerOn = hParam.powerOn;
    this->headParam.speedRear = hParam.speedRear;
    this->headParam.speedFront = hParam.speedFront;
    this->headParam.heatPower = hParam.heatPower;
    this->headParam.heatTime1 = hParam.heatTime1;
    this->headParam.heatTime2 = hParam.heatTime2;
    this->headParam.limitFront = hParam.limitFront;
    this->headParam.limitRear = hParam.limitRear;
    this->headParam.stroksCount = hParam.stroksCount;
}

HeadSetting::~HeadSetting()
{

}

QByteArray IndexerLiftSettings::LiftParameters_::toByteArray()
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

QByteArray IndexerLiftSettings::IndexParameters_::toByteArray()
{
    QByteArray bArr;
    bArr.resize(14);
    bArr[0] = (char)(this->distance&0x00FF);
    bArr[1] = (char)(((this->distance&0xFF00)>>8)&0x00FF);
    bArr[2] = (char)(this->homeOffset&0x00FF);
    bArr[3] = (char)(((this->homeOffset&0xFF00)>>8)&0x00FF);
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

IndexerLiftSettings::IndexerLiftSettings(IndexerLiftSettings::IndexParameters indParam, IndexerLiftSettings::LiftParameters lifParam)
{
    this->indexerParam.acceleration = indParam.acceleration;
    this->indexerParam.accelerationRet = indParam.accelerationRet;
    this->indexerParam.distance = indParam.distance;
    this->indexerParam.distOffcet = indParam.distOffcet;
    this->indexerParam.homeOffset = indParam.homeOffset;
    this->indexerParam.speed = indParam.speed;
    this->indexerParam.speedRet = indParam.speedRet;

    this->liftParam.acceleration = lifParam.acceleration;
    this->liftParam.delayDown = lifParam.delayDown;
    this->liftParam.delayUp = lifParam.delayUp;
    this->liftParam.distance = lifParam.distance;
    this->liftParam.homeOffcet = lifParam.homeOffcet;
    this->liftParam.speed = lifParam.speed;

}

IndexerLiftSettings::IndexerLiftSettings()
{
    IndexParameters indParam;
    this->indexerParam.acceleration = indParam.acceleration;
    this->indexerParam.accelerationRet = indParam.accelerationRet;
    this->indexerParam.distance = indParam.distance;
    this->indexerParam.distOffcet = indParam.distOffcet;
    this->indexerParam.homeOffset = indParam.homeOffset;
    this->indexerParam.speed = indParam.speed;
    this->indexerParam.speedRet = indParam.speedRet;

    LiftParameters lifParam;
    this->liftParam.acceleration = lifParam.acceleration;
    this->liftParam.delayDown = lifParam.delayDown;
    this->liftParam.delayUp = lifParam.delayUp;
    this->liftParam.distance = lifParam.distance;
    this->liftParam.homeOffcet = lifParam.homeOffcet;
    this->liftParam.speed = lifParam.speed;
}

void IndexerLiftSettings::fromByteArray(QByteArray indParamArr, QByteArray lifParamArr)
{
    this->indexerParam.distance = ((0x00FF&((uint16_t)indParamArr[1]))<<8)|(0x00FF&((uint16_t)indParamArr[0]));
    this->indexerParam.homeOffset = ((0x00FF&((uint16_t)indParamArr[3]))<<8)|(0x00FF&((uint16_t)indParamArr[2]));
    this->indexerParam.distOffcet = ((0x00FF&((uint16_t)indParamArr[5]))<<8)|(0x00FF&((uint16_t)indParamArr[4]));
    this->indexerParam.speed = ((0x00FF&((uint16_t)indParamArr[7]))<<8)|(0x00FF&((uint16_t)indParamArr[6]));
    this->indexerParam.acceleration = ((0x00FF&((uint16_t)indParamArr[9]))<<8)|(0x00FF&((uint16_t)indParamArr[8]));
    this->indexerParam.speedRet = ((0x00FF&((uint16_t)indParamArr[11]))<<8)|(0x00FF&((uint16_t)indParamArr[10]));
    this->indexerParam.accelerationRet = ((0x00FF&((uint16_t)indParamArr[13]))<<8)|(0x00FF&((uint16_t)indParamArr[12]));

    this->liftParam.distance = ((0x00FF&((uint16_t)lifParamArr[1]))<<8)|(0x00FF&((uint16_t)lifParamArr[0]));
    this->liftParam.homeOffcet = ((0x00FF&((uint16_t)lifParamArr[3]))<<8)|(0x00FF&((uint16_t)lifParamArr[2]));
    this->liftParam.speed = ((0x00FF&((uint16_t)lifParamArr[5]))<<8)|(0x00FF&((uint16_t)lifParamArr[4]));
    this->liftParam.acceleration = ((0x00FF&((uint16_t)lifParamArr[7]))<<8)|(0x00FF&((uint16_t)lifParamArr[6]));
    this->liftParam.delayDown = ((0x00FF&((uint16_t)lifParamArr[9]))<<8)|(0x00FF&((uint16_t)lifParamArr[8]));
    this->liftParam.delayUp = ((0x00FF&((uint16_t)lifParamArr[11]))<<8)|(0x00FF&((uint16_t)lifParamArr[10]));
}
