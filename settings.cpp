#include "settings.h"
#include <QDebug>

uint32_t HeadSetting::headStateAll;

void HeadSetting::fromByteArray(QByteArray hParamArr)
{
    if(hParamArr.length()!=44)
        hParamArr.resize(44);

    this->headParam.headType = (HeadSetting::HeadType)((uint8_t)hParamArr[0]);
    this->headParam.powerOn = (bool)(hParamArr[1]&0x01);
    this->headParam.speedRear = ((0x00FF&((uint16_t)hParamArr[3]))<<8)|(0x00FF&((uint16_t)hParamArr[2]));
    this->headParam.speedFront = ((0x00FF&((uint16_t)hParamArr[5]))<<8)|(0x00FF&((uint16_t)hParamArr[4]));
    this->headParam.limitRear = ((0x00FF&((uint16_t)hParamArr[7]))<<8)|(0x00FF&((uint16_t)hParamArr[6]));
    this->headParam.limitFront = ((0x00FF&((uint16_t)hParamArr[9]))<<8)|(0x00FF&((uint16_t)hParamArr[8]));
    this->headParam.stroksCount = ((0x00FF&((uint16_t)hParamArr[11]))<<8)|(0x00FF&((uint16_t)hParamArr[10]));
    this->headParam.stroksSBCount = ((0x00FF&((uint16_t)hParamArr[13]))<<8)|(0x00FF&((uint16_t)hParamArr[12]));
    this->headParam.dwellFLTime = ((0x00FF&((uint16_t)hParamArr[15]))<<8)|(0x00FF&((uint16_t)hParamArr[14]));
    this->headParam.dwellSQTime = ((0x00FF&((uint16_t)hParamArr[17]))<<8)|(0x00FF&((uint16_t)hParamArr[16]));
    this->headParam.heatTime1 = ((0x00FF&((uint16_t)hParamArr[19]))<<8)|(0x00FF&((uint16_t)hParamArr[18]));
    this->headParam.heatTime2 = ((0x00FF&((uint16_t)hParamArr[21]))<<8)|(0x00FF&((uint16_t)hParamArr[20]));
    this->headParam.heatPower = ((0x00FF&((uint16_t)hParamArr[23]))<<8)|(0x00FF&((uint16_t)hParamArr[22]));
    this->headParam.limitFront = ((0x00FF&((uint16_t)hParamArr[25]))<<8)|(0x00FF&((uint16_t)hParamArr[24]));
    this->headParam.heatTime1Q = ((0x00FF&((uint16_t)hParamArr[27]))<<8)|(0x00FF&((uint16_t)hParamArr[26]));
    this->headParam.heatTime2Q = ((0x00FF&((uint16_t)hParamArr[29]))<<8)|(0x00FF&((uint16_t)hParamArr[28]));
    this->headParam.dryPowerQ = ((0x00FF&((uint16_t)hParamArr[31]))<<8)|(0x00FF&((uint16_t)hParamArr[30]));
    this->headParam.stepbackDryTimeQ = ((0x00FF&((uint16_t)hParamArr[33]))<<8)|(0x00FF&((uint16_t)hParamArr[32]));
    this->headParam.temperatureSetQ = ((0x00FF&((uint16_t)hParamArr[35]))<<8)|(0x00FF&((uint16_t)hParamArr[34]));
    this->headParam.dryTimeQ = ((0x00FF&((uint16_t)hParamArr[37]))<<8)|(0x00FF&((uint16_t)hParamArr[36]));
    this->headParam.standbyTimeQ = ((0x00FF&((uint16_t)hParamArr[39]))<<8)|(0x00FF&((uint16_t)hParamArr[38]));
    this->headParam.standbyPowerQ = ((0x00FF&((uint16_t)hParamArr[41]))<<8)|(0x00FF&((uint16_t)hParamArr[40]));
    this->headParam.warmFlashTimeQ = ((0x00FF&((uint16_t)hParamArr[43]))<<8)|(0x00FF&((uint16_t)hParamArr[42]));

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
    hp.dwellFLTime = hParam.dwellFLTime;
    hp.dwellSQTime = hParam.dwellSQTime;
    hp.speedFront = hParam.speedFront;
    hp.speedRear = hParam.speedRear;
    hp.stroksCount = hParam.stroksCount;
    hp.stroksSBCount = hParam.stroksSBCount;
    hp.heatTime1Q = hParam.heatTime1Q;
    hp.heatTime2Q = hParam.heatTime2Q;
    hp.dryPowerQ = hParam.dryPowerQ;
    hp.stepbackDryTimeQ = hParam.stepbackDryTimeQ;
    hp.temperatureSetQ = hParam.temperatureSetQ;
    hp.dryTimeQ = hParam.dryTimeQ;
    hp.standbyTimeQ = hParam.standbyTimeQ;
    hp.standbyPowerQ = hParam.standbyPowerQ;
    hp.warmFlashTimeQ = hParam.warmFlashTimeQ;
    return hp;
}

QByteArray HeadSetting::HeadParameters_::toByteArray()
{
    QByteArray bArr;
    bArr.resize(43);
    bArr[0] = this->headType;
    bArr[1] = (char)(this->powerOn&0x01);
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
    bArr[12] = (char)(this->stroksSBCount&0x00FF);
    bArr[13] = (char)(((this->stroksSBCount&0xFF00)>>8)&0x00FF);
    bArr[14] = (char)(this->dwellFLTime&0x00FF);
    bArr[15] = (char)(((this->dwellFLTime&0xFF00)>>8)&0x00FF);
    bArr[16] = (char)(this->dwellSQTime&0x00FF);
    bArr[17] = (char)(((this->dwellSQTime&0xFF00)>>8)&0x00FF);
    bArr[18] = (char)(this->heatTime1&0x00FF);
    bArr[19] = (char)(((this->heatTime1&0xFF00)>>8)&0x00FF);
    bArr[20] = (char)(this->heatTime2&0x00FF);
    bArr[21] = (char)(((this->heatTime2&0xFF00)>>8)&0x00FF);
    bArr[22] = (char)(this->heatPower&0x00FF);
    bArr[23] = (char)(((this->heatPower&0xFF00)>>8)&0x00FF);
    bArr[24] = (char)(this->limitFront&0x00FF);
    bArr[25] = (char)(((this->limitFront&0xFF00)>>8)&0x00FF);
    bArr[26] = (char)(this->heatTime1Q&0x00FF);
    bArr[27] = (char)(((this->heatTime1Q&0xFF00)>>8)&0x00FF);
    bArr[28] = (char)(this->heatTime2Q&0x00FF);
    bArr[29] = (char)(((this->heatTime2Q&0xFF00)>>8)&0x00FF);
    bArr[30] = (char)(this->dryPowerQ&0x00FF);
    bArr[31] = (char)(((this->dryPowerQ&0xFF00)>>8)&0x00FF);
    bArr[32] = (char)(this->stepbackDryTimeQ&0x00FF);
    bArr[33] = (char)(((this->stepbackDryTimeQ&0xFF00)>>8)&0x00FF);
    bArr[34] = (char)(this->temperatureSetQ&0x00FF);
    bArr[35] = (char)(((this->temperatureSetQ&0xFF00)>>8)&0x00FF);
    bArr[36] = (char)(this->dryTimeQ&0x00FF);
    bArr[37] = (char)(((this->dryTimeQ&0xFF00)>>8)&0x00FF);
    bArr[38] = (char)(this->standbyTimeQ&0x00FF);
    bArr[39] = (char)(((this->standbyTimeQ&0xFF00)>>8)&0x00FF);
    bArr[40] = (char)(this->standbyPowerQ&0x00FF);
    bArr[41] = (char)(((this->standbyPowerQ&0xFF00)>>8)&0x00FF);
    bArr[42] = (char)(this->warmFlashTimeQ&0x00FF);
    bArr[43] = (char)(((this->warmFlashTimeQ&0xFF00)>>8)&0x00FF);

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
    this->headParam.stroksSBCount = hParam.stroksSBCount;
    this->headParam.dwellFLTime = hParam.dwellFLTime;
    this->headParam.dwellSQTime = hParam.dwellSQTime;
    this->headParam.heatTime1Q = hParam.heatTime1Q;
    this->headParam.heatTime2Q = hParam.heatTime2Q;
    this->headParam.dryPowerQ = hParam.dryPowerQ;
    this->headParam.stepbackDryTimeQ = hParam.stepbackDryTimeQ;
    this->headParam.temperatureSetQ = hParam.temperatureSetQ;
    this->headParam.dryTimeQ = hParam.dryTimeQ;
    this->headParam.standbyTimeQ = hParam.standbyTimeQ;
    this->headParam.standbyPowerQ = hParam.standbyPowerQ;
    this->headParam.warmFlashTimeQ = hParam.warmFlashTimeQ;

}

HeadSetting::HeadSetting()
{
    this->headParam.headType = HeadSetting::PrintHead;
    this->headParam.powerOn = 0;
    this->headParam.speedRear = 10;
    this->headParam.speedFront = 10;
    this->headParam.heatPower = 50;
    this->headParam.heatTime1 = 2;
    this->headParam.heatTime2 = 2;
    this->headParam.limitFront = 1;
    this->headParam.limitRear = 1;
    this->headParam.stroksCount = 1;
    this->headParam.stroksSBCount = 0;
    this->headParam.dwellFLTime = 1;
    this->headParam.dwellSQTime = 1;
    this->headParam.heatTime1Q = 0;
    this->headParam.heatTime2Q = 0;
    this->headParam.dryPowerQ = 0;
    this->headParam.stepbackDryTimeQ = 0;
    this->headParam.temperatureSetQ = 0;
    this->headParam.dryTimeQ = 0;
    this->headParam.standbyTimeQ = 0;
    this->headParam.standbyPowerQ = 0;
    this->headParam.warmFlashTimeQ = 0;
}

HeadSetting::~HeadSetting()
{

}

uint16_t HeadSetting::getHeadStateLo()
{
    return static_cast<uint16_t>((HeadSetting::headStateAll)&0x0000FFFF);
}

uint16_t HeadSetting::getHeadStateHi()
{
    return static_cast<uint16_t>(((HeadSetting::headStateAll)>>16)&0x0000FFFF);
}

bool HeadSetting::getHeadStateAtIndex(uint8_t index)
{
    return static_cast<bool>((HeadSetting::headStateAll)&(1<<index));
}

void HeadSetting::setHeadStateAtIndex(uint8_t index, bool state)
{
    if(state)
        HeadSetting::headStateAll |= (1<<(index));
    else
        HeadSetting::headStateAll &= (~(1<<index));
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
    this->indexerParam.acceleration = 100;
    this->indexerParam.accelerationRet = 100;
    this->indexerParam.distance = 5;
    this->indexerParam.distOffcet = 0;
    this->indexerParam.homeOffset = 0;
    this->indexerParam.speed = 100;
    this->indexerParam.speedRet = 100;

    this->liftParam.acceleration = 100;
    this->liftParam.delayDown = 1;
    this->liftParam.delayUp = 1;
    this->liftParam.distance = 5;
    this->liftParam.homeOffcet = 0;
    this->liftParam.speed = 100;
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


bool MachineSettings::serviceWidgetsEn;
MachineSettings::MachineType MachineSettings::machineTypeStat;

QByteArray MachineSettings::MachineParameters_::toByteArray()
{
    QByteArray bArr;
    bArr.resize(6);
    bArr[0] = (char)(this->headCount&0x00FF);
    bArr[1] = (char)(((this->headCount&0xFF00)>>8)&0x00FF);
    bArr[2] = (char)(this->warningTime&0x00FF);
    bArr[3] = (char)(((this->warningTime&0xFF00)>>8)&0x00FF);
    bArr[4] = (char)(this->direction&0x00FF);
    bArr[5] = (char)(((this->direction&0xFF00)>>8)&0x00FF);
    bArr[6] = (char)(this->machineType&0x00FF);
    bArr[7] = (char)(((this->machineType&0xFF00)>>8)&0x00FF);
    return bArr;
}

MachineSettings::MachineSettings(MachineSettings::MachineParameters mParam)
{
    machineTypeList<<"VoltServo"<<"VoltAC"  <<"Vector"  <<"TitanASE"<<"TitanASA"<<"TitanAAA";
    machineTypeData<<0x0111     <<0x0121    <<0x0112    <<0x0113    <<0x0213    <<0x0333;
    this->machineParam.headCount = mParam.headCount;
    this->machineParam.warningTime = mParam.headCount;
    this->machineParam.direction = mParam.direction;
    this->machineParam.machineType = mParam.machineType;
    this->machineTypeStat = mParam.machineType;
}

MachineSettings::MachineSettings()
{
    machineTypeList<<"VoltServo"<<"VoltAC"  <<"Vector"  <<"TitanASE"<<"TitanASA"<<"TitanAAA";
    machineTypeData<<0x0111     <<0x0121    <<0x0112    <<0x0113    <<0x0213    <<0x0333;
    this->machineParam.headCount = 6;
    this->machineParam.warningTime = 1;
    this->machineParam.direction = 1;
    this->machineParam.machineType = MachineSettings::Vector;
}

void MachineSettings::fromByteArray(QByteArray machineParamArray)
{
    this->machineParam.headCount = ((0x00FF&((uint16_t)machineParamArray[1]))<<8)
            |(0x00FF&((uint16_t)machineParamArray[0]));
    this->machineParam.warningTime = ((0x00FF&((uint16_t)machineParamArray[3]))<<8)
            |(0x00FF&((uint16_t)machineParamArray[2]));
    this->machineParam.direction = ((0x00FF&((uint16_t)machineParamArray[5]))<<8)
            |(0x00FF&((uint16_t)machineParamArray[4]));
    this->machineParam.machineType = static_cast<MachineSettings::MachineType>
            (((0x00FF&((uint16_t)machineParamArray[7]))<<8)
            |(0x00FF&((uint16_t)machineParamArray[6])));
    this->machineTypeStat = this->machineParam.machineType;
}

bool MachineSettings::getServiceWidgEn()
{
    MachineSettings stt;
    return stt.serviceWidgetsEn;
}

void MachineSettings::setServiceWidgEn(bool servEn)
{
    MachineSettings stt;
    stt.serviceWidgetsEn = servEn;
}

MachineSettings::MachineType MachineSettings::getMachineType()
{
    MachineSettings stt;
    return stt.machineTypeStat;
}

void MachineSettings::setMachineType(MachineSettings::MachineType mType)
{
    MachineSettings stt;
    stt.machineTypeStat = mType;
}
