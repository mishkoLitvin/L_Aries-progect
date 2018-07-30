#include "settings.h"
#include <QDebug>

uint32_t HeadSetting::headStateAll;

void HeadSetting::fromByteArray(QByteArray hParamArr)
{
    if(hParamArr.length()!=45)
        hParamArr.resize(45);

    this->headParam.headOnType = (HeadSetting::HeadOnType)(((0x00FF&((uint16_t)hParamArr[1]))<<8)|(0x00FF&((uint16_t)hParamArr[0])));;
    this->headParam.powerOn = ((uint8_t)hParamArr[2]);
    this->headParam.speedRear = ((0x00FF&((uint16_t)hParamArr[4]))<<8)|(0x00FF&((uint16_t)hParamArr[3]));
    this->headParam.speedFront = ((0x00FF&((uint16_t)hParamArr[6]))<<8)|(0x00FF&((uint16_t)hParamArr[5]));
    this->headParam.limitRear = ((0x00FF&((uint16_t)hParamArr[8]))<<8)|(0x00FF&((uint16_t)hParamArr[7]));
    this->headParam.limitFront = ((0x00FF&((uint16_t)hParamArr[10]))<<8)|(0x00FF&((uint16_t)hParamArr[9]));
    this->headParam.stroksCount = ((0x00FF&((uint16_t)hParamArr[12]))<<8)|(0x00FF&((uint16_t)hParamArr[11]));
    this->headParam.stroksSBCount = ((0x00FF&((uint16_t)hParamArr[14]))<<8)|(0x00FF&((uint16_t)hParamArr[13]));
    this->headParam.dwellFLTime = ((0x00FF&((uint16_t)hParamArr[16]))<<8)|(0x00FF&((uint16_t)hParamArr[15]));
    this->headParam.dwellSQTime = ((0x00FF&((uint16_t)hParamArr[18]))<<8)|(0x00FF&((uint16_t)hParamArr[17]));
    this->headParam.heatTime1 = ((0x00FF&((uint16_t)hParamArr[20]))<<8)|(0x00FF&((uint16_t)hParamArr[19]));
    this->headParam.heatTime2 = ((0x00FF&((uint16_t)hParamArr[22]))<<8)|(0x00FF&((uint16_t)hParamArr[21]));
    this->headParam.heatPower = ((0x00FF&((uint16_t)hParamArr[24]))<<8)|(0x00FF&((uint16_t)hParamArr[23]));
    this->headParam.limitFront = ((0x00FF&((uint16_t)hParamArr[26]))<<8)|(0x00FF&((uint16_t)hParamArr[25]));
    this->headParam.heatTime1Q = ((0x00FF&((uint16_t)hParamArr[28]))<<8)|(0x00FF&((uint16_t)hParamArr[27]));
    this->headParam.heatTime2Q = ((0x00FF&((uint16_t)hParamArr[30]))<<8)|(0x00FF&((uint16_t)hParamArr[29]));
    this->headParam.dryPowerQ = ((0x00FF&((uint16_t)hParamArr[32]))<<8)|(0x00FF&((uint16_t)hParamArr[31]));
    this->headParam.stepbackDryTimeQ = ((0x00FF&((uint16_t)hParamArr[34]))<<8)|(0x00FF&((uint16_t)hParamArr[33]));
    this->headParam.temperatureSetQ = ((0x00FF&((uint16_t)hParamArr[36]))<<8)|(0x00FF&((uint16_t)hParamArr[35]));
    this->headParam.dryTimeQ = ((0x00FF&((uint16_t)hParamArr[38]))<<8)|(0x00FF&((uint16_t)hParamArr[37]));
    this->headParam.standbyTimeQ = ((0x00FF&((uint16_t)hParamArr[40]))<<8)|(0x00FF&((uint16_t)hParamArr[39]));
    this->headParam.standbyPowerQ = ((0x00FF&((uint16_t)hParamArr[42]))<<8)|(0x00FF&((uint16_t)hParamArr[41]));
    this->headParam.warmFlashTimeQ = ((0x00FF&((uint16_t)hParamArr[44]))<<8)|(0x00FF&((uint16_t)hParamArr[43]));

}

HeadSetting::HeadParameters HeadSetting::operator =(HeadSetting::HeadParameters hParam)
{
    HeadParameters hp;
    hp.headOnType = hParam.headOnType;
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
    bArr.resize(45);
    bArr[0] = static_cast<char>(this->headOnType&0x00FF);
    bArr[1] = static_cast<char>(((this->headOnType&0xFF00)>>8)&0x00FF);
    bArr[2] = static_cast<char>(this->powerOn);
    bArr[3] = static_cast<char>(this->speedRear&0x00FF);
    bArr[4] = static_cast<char>(((this->speedRear&0xFF00)>>8)&0x00FF);
    bArr[5] = static_cast<char>(this->speedFront&0x00FF);
    bArr[6] = static_cast<char>(((this->speedFront&0xFF00)>>8)&0x00FF);
    bArr[7] = static_cast<char>(this->limitRear&0x00FF);
    bArr[8] = static_cast<char>(((this->limitRear&0xFF00)>>8)&0x00FF);
    bArr[9] = static_cast<char>(this->limitFront&0x00FF);
    bArr[10] = static_cast<char>(((this->limitFront&0xFF00)>>8)&0x00FF);
    bArr[11] = static_cast<char>(this->stroksCount&0x00FF);
    bArr[12] = static_cast<char>(((this->stroksCount&0xFF00)>>8)&0x00FF);
    bArr[13] = static_cast<char>(this->stroksSBCount&0x00FF);
    bArr[14] = static_cast<char>(((this->stroksSBCount&0xFF00)>>8)&0x00FF);
    bArr[15] = static_cast<char>(this->dwellFLTime&0x00FF);
    bArr[16] = static_cast<char>(((this->dwellFLTime&0xFF00)>>8)&0x00FF);
    bArr[17] = static_cast<char>(this->dwellSQTime&0x00FF);
    bArr[18] = static_cast<char>(((this->dwellSQTime&0xFF00)>>8)&0x00FF);
    bArr[19] = static_cast<char>(this->heatTime1&0x00FF);
    bArr[20] = static_cast<char>(((this->heatTime1&0xFF00)>>8)&0x00FF);
    bArr[21] = static_cast<char>(this->heatTime2&0x00FF);
    bArr[22] = static_cast<char>(((this->heatTime2&0xFF00)>>8)&0x00FF);
    bArr[23] = static_cast<char>(this->heatPower&0x00FF);
    bArr[24] = static_cast<char>(((this->heatPower&0xFF00)>>8)&0x00FF);
    bArr[25] = static_cast<char>(this->limitFront&0x00FF);
    bArr[26] = static_cast<char>(((this->limitFront&0xFF00)>>8)&0x00FF);
    bArr[27] = static_cast<char>(this->heatTime1Q&0x00FF);
    bArr[28] = static_cast<char>(((this->heatTime1Q&0xFF00)>>8)&0x00FF);
    bArr[29] = static_cast<char>(this->heatTime2Q&0x00FF);
    bArr[30] = static_cast<char>(((this->heatTime2Q&0xFF00)>>8)&0x00FF);
    bArr[31] = static_cast<char>(this->dryPowerQ&0x00FF);
    bArr[32] = static_cast<char>(((this->dryPowerQ&0xFF00)>>8)&0x00FF);
    bArr[33] = static_cast<char>(this->stepbackDryTimeQ&0x00FF);
    bArr[34] = static_cast<char>(((this->stepbackDryTimeQ&0xFF00)>>8)&0x00FF);
    bArr[35] = static_cast<char>(this->temperatureSetQ&0x00FF);
    bArr[36] = static_cast<char>(((this->temperatureSetQ&0xFF00)>>8)&0x00FF);
    bArr[37] = static_cast<char>(this->dryTimeQ&0x00FF);
    bArr[38] = static_cast<char>(((this->dryTimeQ&0xFF00)>>8)&0x00FF);
    bArr[39] = static_cast<char>(this->standbyTimeQ&0x00FF);
    bArr[40] = static_cast<char>(((this->standbyTimeQ&0xFF00)>>8)&0x00FF);
    bArr[41] = static_cast<char>(this->standbyPowerQ&0x00FF);
    bArr[42] = static_cast<char>(((this->standbyPowerQ&0xFF00)>>8)&0x00FF);
    bArr[43] = static_cast<char>(this->warmFlashTimeQ&0x00FF);
    bArr[44] = static_cast<char>(((this->warmFlashTimeQ&0xFF00)>>8)&0x00FF);

    return bArr;
}

HeadSetting::HeadSetting(HeadParameters hParam)
{
    this->headParam.headOnType = hParam.headOnType;
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
    this->headParam.headOnType = HeadSetting::PrintHeadOff;
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

void HeadSetting::setHeadOn_OffStateInd(uint8_t index, bool state)
{
    if(state)
        HeadSetting::headStateAll &= (~(1<<(index)));
    else
        HeadSetting::headStateAll |= ((1<<index));
}

QByteArray IndexerLiftSettings::LiftParameters_::toByteArray()
{
    QByteArray bArr;
    bArr.resize(12);
    bArr[0] = static_cast<char>(this->distance&0x00FF);
    bArr[1] = static_cast<char>(((this->distance&0xFF00)>>8)&0x00FF);
    bArr[2] = static_cast<char>(this->homeOffcet&0x00FF);
    bArr[3] = static_cast<char>(((this->homeOffcet&0xFF00)>>8)&0x00FF);
    bArr[4] = static_cast<char>(this->speed&0x00FF);
    bArr[5] = static_cast<char>(((this->speed&0xFF00)>>8)&0x00FF);
    bArr[6] = static_cast<char>(this->acceleration&0x00FF);
    bArr[7] = static_cast<char>(((this->acceleration&0xFF00)>>8)&0x00FF);
    bArr[8] = static_cast<char>(this->delayDown&0x00FF);
    bArr[9] = static_cast<char>(((this->delayDown&0xFF00)>>8)&0x00FF);
    bArr[10] = static_cast<char>(this->delayUp&0x00FF);
    bArr[11] = static_cast<char>(((this->delayUp&0xFF00)>>8)&0x00FF);

    return bArr;
}

QByteArray IndexerLiftSettings::IndexParameters_::toByteArray()
{
    QByteArray bArr;
    bArr.resize(14);
    bArr[0] = static_cast<char>(this->distance&0x00FF);
    bArr[1] = static_cast<char>(((this->distance&0xFF00)>>8)&0x00FF);
    bArr[2] = static_cast<char>(this->homeOffset&0x00FF);
    bArr[3] = static_cast<char>(((this->homeOffset&0xFF00)>>8)&0x00FF);
    bArr[4] = static_cast<char>(this->distOffcet&0x00FF);
    bArr[5] = static_cast<char>(((this->distOffcet&0xFF00)>>8)&0x00FF);
    bArr[6] = static_cast<char>(this->speed&0x00FF);
    bArr[7] = static_cast<char>(((this->speed&0xFF00)>>8)&0x00FF);
    bArr[8] = static_cast<char>(this->acceleration&0x00FF);
    bArr[9] = static_cast<char>(((this->acceleration&0xFF00)>>8)&0x00FF);
    bArr[10] = static_cast<char>(this->speedRet&0x00FF);
    bArr[11] = static_cast<char>(((this->speedRet&0xFF00)>>8)&0x00FF);
    bArr[12] = static_cast<char>(this->accelerationRet&0x00FF);
    bArr[13] = static_cast<char>(((this->accelerationRet&0xFF00)>>8)&0x00FF);

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
uint16_t MachineSettings::headMaxRangeStat;
uint16_t MachineSettings::headTypeStat;
uint16_t MachineSettings::indexerLiftTypeStat;

QByteArray MachineSettings::MachineParameters_::toByteArray()
{
    QByteArray bArr;
    bArr.resize(21);
    bArr[0] = static_cast<char>(this->headCount&0x00FF);
    bArr[1] = static_cast<char>(((this->headCount&0xFF00)>>8)&0x00FF);
    bArr[2] = static_cast<char>(this->warningTime&0x00FF);
    bArr[3] = static_cast<char>(((this->warningTime&0xFF00)>>8)&0x00FF);
    bArr[4] = static_cast<char>(this->direction&0x00FF);
    bArr[5] = static_cast<char>(((this->direction&0xFF00)>>8)&0x00FF);
    bArr[6] = static_cast<char>(this->machineType&0x00FF);
    bArr[7] = static_cast<char>(((this->machineType&0xFF00)>>8)&0x00FF);
    bArr[8] = static_cast<char>(this->headType.all&0x00FF);
    bArr[9] = static_cast<char>(((this->headType.all&0xFF00)>>8)&0x00FF);
    bArr[10] = static_cast<char>(this->indexeLiftType.all&0x00FF);
    bArr[11] = static_cast<char>(((this->indexeLiftType.all&0xFF00)>>8)&0x00FF);
    bArr[12] = static_cast<char>(this->headMaxRange&0x00FF);
    bArr[13] = static_cast<char>(((this->headMaxRange&0xFF00)>>8)&0x00FF);
    bArr[14] = static_cast<char>(this->liftGearRatio&0x00FF);
    bArr[15] = static_cast<char>(((this->liftGearRatio&0xFF00)>>8)&0x00FF);
    bArr[16] = static_cast<char>(this->indexerScrewPinch&0x00FF);
    bArr[17] = static_cast<char>(((this->indexerScrewPinch&0xFF00)>>8)&0x00FF);
    bArr[18] = static_cast<char>(this->useUnloadHead&0x00FF);
    bArr[19] = static_cast<char>(this->stepTimeDelay&0x00FF);
    bArr[20] = static_cast<char>(this->lastRevWarm.all&0x00FF);
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
    this->machineParam.headType = mParam.headType;
    this->machineParam.indexeLiftType = mParam.indexeLiftType;
    this->machineParam.headMaxRange = mParam.headMaxRange;
    this->machineParam.liftGearRatio = mParam.liftGearRatio;
    this->machineParam.indexerScrewPinch = mParam.indexerScrewPinch;
    this->machineParam.useUnloadHead = mParam.useUnloadHead;
    this->machineParam.stepTimeDelay = mParam.stepTimeDelay;
    this->machineParam.lastRevWarm.all = mParam.lastRevWarm.all;
}

MachineSettings::MachineSettings()
{
    machineTypeList<<"VoltServo"<<"VoltAC"  <<"Vector"  <<"TitanASE"<<"TitanASA"<<"TitanAAA";
    machineTypeData<<0x0111     <<0x0121    <<0x0112    <<0x0113    <<0x0213    <<0x0333;
    this->machineParam.headCount = 6;
    this->machineParam.warningTime = 1;
    this->machineParam.direction = 1;
    this->machineParam.machineType = MachineSettings::Vector;
    this->machineParam.headType.all = 0;
    this->machineParam.indexeLiftType.all = 0;
    this->machineParam.headMaxRange = 300;
    this->machineParam.liftGearRatio = 30;
    this->machineParam.indexerScrewPinch = 10;
    this->machineParam.useUnloadHead = false;
    this->machineParam.useUnloadHead = 10;

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
    this->machineParam.headType.all =  (((0x00FF&((uint16_t)machineParamArray[9]))<<8)
            |(0x00FF&((uint16_t)machineParamArray[8])));
    this->machineParam.indexeLiftType.all = (((0x00FF&((uint16_t)machineParamArray[11]))<<8)
            |(0x00FF&((uint16_t)machineParamArray[10])));
    this->machineParam.headMaxRange = (((0x00FF&((uint16_t)machineParamArray[13]))<<8)
            |(0x00FF&((uint16_t)machineParamArray[12])));
    this->machineParam.liftGearRatio = (((0x00FF&((uint16_t)machineParamArray[15]))<<8)
            |(0x00FF&((uint16_t)machineParamArray[14])));
    this->machineParam.indexerScrewPinch = (((0x00FF&((uint16_t)machineParamArray[17]))<<8)
            |(0x00FF&((uint16_t)machineParamArray[16])));
    this->machineParam.useUnloadHead = (bool)(0x00FF&((uint16_t)machineParamArray[18]));
    this->machineParam.stepTimeDelay = (0x00FF&((uint8_t)machineParamArray[19]));
    this->machineParam.lastRevWarm.all = (0x00FF&((uint8_t)machineParamArray[20]));
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
    return MachineSettings::machineTypeStat;
}

void MachineSettings::setMachineType(MachineSettings::MachineType mType)
{
    MachineSettings::machineTypeStat = mType;
}

uint16_t MachineSettings::getHeadMaxRange()
{
    return MachineSettings::headMaxRangeStat;
}

uint16_t MachineSettings::getHeadType()
{
    return MachineSettings::headTypeStat;
}

uint16_t MachineSettings::getIndexLiftType()
{
    return MachineSettings::indexerLiftTypeStat;
}

void MachineSettings::setHeadMaxRange(uint16_t val)
{
    MachineSettings::headMaxRangeStat = val;
}

void MachineSettings::setHeadType(uint16_t val)
{
    MachineSettings::headTypeStat = val;
}

void MachineSettings::setIndexLiftType(uint16_t val)
{
    MachineSettings::indexerLiftTypeStat = val;
}

Register::Register(uint16_t headCount)
{
    masterRegPtr = &(masterReg.memBeg);
    indexerRegPtr = &(indexerReg.memBeg);
    liftRegPtr = &(liftReg.memBeg);

    uint16_t i;
    for(i = 0; i<headCount; i++)
    {
        headRegList.append(*(new HeadReg));
        headRegPtrList.append(&(headRegList[i].memBeg));
    }

    for(i = 0; i < sizeof(MasterReg); i++)
    {
        *(i+masterRegPtr) = i;
    }

}

void Register::writeReg(uint8_t dev, uint8_t place, uint16_t data)
{
    if(dev<=HeadSetting::HeadDeviceAdrOffcet)
        switch (dev) {
        case MachineSettings::MasterDevice:
            if(place<37)
                *(masterRegPtr+place) = data;
            break;
        case IndexerLiftSettings::IndexerDevice:
            if(place<37)
                *(indexerRegPtr+place) = data;
            break;
        case IndexerLiftSettings::LiftDevice:
            if(place<36)
                *(liftRegPtr+place) = data;
            break;
        }
    else
    {
        int i = dev - HeadSetting::HeadDeviceAdrOffcet;
        if((i<headRegPtrList.length())&(place<44))
            *(headRegPtrList[i]+place) = data;
    }

}

uint16_t Register::readReg(uint8_t dev, uint8_t place)
{
    uint16_t data;
    if(dev<=HeadSetting::HeadDeviceAdrOffcet)
        switch (dev) {
        case MachineSettings::MasterDevice:
            data = *(masterRegPtr+place);
            break;
        case IndexerLiftSettings::IndexerDevice:
            data = *(indexerRegPtr+place);
            break;
        case IndexerLiftSettings::LiftDevice:
            data = *(liftRegPtr+place);
            break;
        }
    else
    {
        int i = dev - HeadSetting::HeadDeviceAdrOffcet;
        if(i<headRegPtrList.length())
            data = *(headRegPtrList[i]+place);
        else
            data = 0x0000;
    }
    return data;
}

void Register::setMasterReg(MachineSettings mSett)
{
    this->machineSettings = mSett;
    this->masterReg.field.masterReg_DEV_INF_H = 0;
    this->masterReg.field.masterReg_SIZE = mSett.machineParam.headCount;
    this->masterReg.field.masterReg_PRZ = 0;
    this->masterReg.field.masterReg_STA = 0;
    this->masterReg.field.masterheadReg = 0;
    this->masterReg.field.masterReg_LAM = 0;
    this->masterReg.field.masterReg_TOTALH = 0;
    this->masterReg.field.masterReg_TOTALL = 0;
    this->masterReg.field.masterheadReg1 = 0;
    this->masterReg.field.masterReg_PAL = 0;
    this->masterReg.field.masterReg_INPUT = 0;
    this->masterReg.field.masterReg_REMAINH = 0;
    this->masterReg.field.masterReg_REMAINL = 0;
    this->masterReg.field.masterReg_SPEED = 0;
    this->masterReg.field.masterReg_PRINTEDH = 0;
    this->masterReg.field.masterReg_PRINTEDL = 0;
    this->masterReg.field.masterReg_MACHINE_TYPE = mSett.machineParam.machineType;
    this->masterReg.field.masterReg_PAL1 = 0;
    this->masterReg.field.masterReg_EKR = 0;
    this->masterReg.field.masterReg_ACTIVHEAD_L = 0;
    this->masterReg.field.masterReg_ACTIVHEAD_H = 0;
    this->masterReg.field.masterReg_DEVERR = 0;
    this->masterReg.field.masterReg_ERR = 0;
    this->masterReg.field.masterReg_KODH = 0;
    this->masterReg.field.masterReg_KODL = 0;
    this->masterReg.field.masterReg_DATH = 0;
    this->masterReg.field.masterReg_DATL = 0;
    this->masterReg.field.masterReg_KOD_ON = 0;
    this->masterReg.field.masterReg_DAT = (uint16_t)(((uint16_t)((QDate::currentDate().year()-2000)<<9))
                                                     |((uint16_t)(QDate::currentDate().month()<<5)
                                                       |(uint16_t)(QDate::currentDate().day())));
    this->masterReg.field.REG_KOD_WPISZ = 0;
    this->masterReg.field.masterReg_HRW = 0;
    this->masterReg.field.masterReg_HRW1 = 0;
    this->masterReg.field.masterReg_KOD_ON2 = 0;
    this->masterReg.field.masterReg_KOD_ON3 = 0;
    this->masterReg.field.masterReg_ERROR_MESSAGE = 0;
    this->masterReg.field.masterReg_DEV_INF_L = 0;
}

void Register::setHeadReg(int index, HeadSetting hSett)
{
    this->headRegList[index].field.REG_DEV_INF_H = 0;
    this->headRegList[index].field.headReg_ON = hSett.headParam.headOnType;
    this->headRegList[index].field.headReg_RSPD = hSett.headParam.speedRear;
    this->headRegList[index].field.headReg_FSPD = hSett.headParam.speedFront;
    this->headRegList[index].field.headReg_FLDWE = hSett.headParam.dwellFLTime;
    this->headRegList[index].field.headReg_SQDWE = hSett.headParam.dwellSQTime;
    this->headRegList[index].field.headReg_NOSTR = hSett.headParam.stroksCount;
    this->headRegList[index].field.headReg_RW1_TIME = hSett.headParam.heatTime1Q;
    this->headRegList[index].field.headReg_RW2_TIME = hSett.headParam.heatTime2Q;
    this->headRegList[index].field.headReg_RW3_TIME = 0;
    this->headRegList[index].field.headReg_REG_INP = 0;
    this->headRegList[index].field.headReg_ROZ = 0;
    this->headRegList[index].field.headReg_STAN = 0;
    this->headRegList[index].field.REG_KEY = 0;
    this->headRegList[index].field.headReg_SBSTR = hSett.headParam.stroksSBCount;
    this->headRegList[index].field.REG_LED = 0;
    this->headRegList[index].field.headRegIsHeat = 0;
    this->headRegList[index].field.headReg_R = 0;
    this->headRegList[index].field.headReg_G = 0;
    this->headRegList[index].field.headReg_B = 0;
    this->headRegList[index].field.REG_SERVO_HOLD = 0;
    this->headRegList[index].field.REG_SHUTTLE_REAR_POS = hSett.headParam.heatLimit;
    this->headRegList[index].field.headReg_RANGE_1 = hSett.headParam.limitFront;
    this->headRegList[index].field.headReg_RANGE_2 = hSett.headParam.limitRear;
    this->headRegList[index].field.REG_TEMP_SET = hSett.headParam.temperatureSetQ;
    this->headRegList[index].field.REG_SENSOR_TIME = hSett.headParam.heatTime1Q;
    this->headRegList[index].field.REG_STANDBY_POWER = hSett.headParam.standbyPowerQ;
    this->headRegList[index].field.REG_STANDBY_TIME = hSett.headParam.standbyTimeQ;
    this->headRegList[index].field.headReg_CONFIG = 0x00;
    this->headRegList[index].field.REG_RW_POWER = hSett.headParam.dryPowerQ;
    this->headRegList[index].field.REG_TEMP_MEAS = 0;
    this->headRegList[index].field.headReg_WARM_PAL_TIME = hSett.headParam.warmFlashTimeQ;
    this->headRegList[index].field.headReg_WARM_FLASH_TIME = hSett.headParam.warmFlashTimeQ;
    this->headRegList[index].field.headReg_MACHINE_TYPE = 0;
    this->headRegList[index].field.headReg_PRESSURE_1 = 0;
    this->headRegList[index].field.headReg_PRESSURE_2 = 0;
    this->headRegList[index].field.headReg_PRESSURE_3 = 0;
    this->headRegList[index].field.headReg_PRESSURE_4 = 0;
    this->headRegList[index].field.headReg_PRESSURE_5 = 0;
    this->headRegList[index].field.headReg_ERROR_MESSAGE = 0;
    this->headRegList[index].field.REG_HMI_DATA = 0;
    this->headRegList[index].field.headReg_REVOLVER_STR_L = 0;
    this->headRegList[index].field.headReg_REVOLVER_STR_H = 0;
    this->headRegList[index].field.REG_DEV_INF_L = 0;
}

void Register::setIndexLiftReg(IndexerLiftSettings iLSett)
{
    this->indexerLiftSettings = iLSett;
    this->indexerReg.field.masterReg_DEV_INF_H = 0;
    this->indexerReg.field.indexerReg_HOME_OFF = iLSett.indexerParam.homeOffset;
    this->indexerReg.field.indexerReg_DIST_OFF = iLSett.indexerParam.distOffcet;
    this->indexerReg.field.indexerReg_MAX_SPEED = iLSett.indexerParam.speed;
    this->indexerReg.field.indexerReg_DIR = this->machineSettings.machineParam.direction;
    this->indexerReg.field.indexerReg_CYCLE_DWELL = 0;
    this->indexerReg.field.indexerliftReg_UP_DELAY = iLSett.liftParam.delayUp;
    this->indexerReg.field.indexerReg_DIST = iLSett.indexerParam.distance;
    this->indexerReg.field.indexerReg_TM = 0;
    this->indexerReg.field.indexerReg_ACC = iLSett.indexerParam.acceleration;
    this->indexerReg.field.indexerReg_RACC = iLSett.indexerParam.accelerationRet;
    this->indexerReg.field.indexerReg_RSPEED = iLSett.indexerParam.speedRet;
    this->indexerReg.field.indexerReg_WARN = 0;
    this->indexerReg.field.liftReg_HOME_OFF = iLSett.liftParam.homeOffcet;
    this->indexerReg.field.liftReg_DIST = iLSett.liftParam.distance;
    this->indexerReg.field.liftReg_SPEED = iLSett.liftParam.speed;
    this->indexerReg.field.liftReg_ACC = iLSett.liftParam.acceleration;
    this->indexerReg.field.indexerReg_HRW_TIME_1 = 0;
    this->indexerReg.field.indexerReg_HRW_TIME_2 = 0;
    this->indexerReg.field.indexerReg_HRW_TIME_3 = 0;
    this->indexerReg.field.indexerReg_HRW_TIME_4 = 0;
    this->indexerReg.field.indexerReg_MODE = 0;
    this->indexerReg.field.indexerReg_LOAD = 0;
    this->indexerReg.field.indexerReg_WARM_CYCLES = 0;
    this->indexerReg.field.indexerReg_WARM_TEMP = 0;
    this->indexerReg.field.indexerReg_WARM_TIME = 0;
    this->indexerReg.field.indexerReg_SKOK_SR = 10;
    this->indexerReg.field.indexerReg_DEV_INF_L = 0;


    this->liftReg.field.liftReg_SEQU1_L = 0;
    this->liftReg.field.liftReg_SEQU1_H = 0;
    this->liftReg.field.liftReg_SEQU2_L = 0;
    this->liftReg.field.liftReg_SEQU2_H = 0;
    this->liftReg.field.liftReg_SEQU3_L = 0;
    this->liftReg.field.liftReg_SEQU3_H = 0;
    this->liftReg.field.liftReg_SEQU4_L = 0;
    this->liftReg.field.liftReg_SEQU4_H = 0;
    this->liftReg.field.liftReg_SEQU5_L = 0;
    this->liftReg.field.liftReg_SEQU5_H = 0;
    this->liftReg.field.liftReg_SEQU6_L = 0;
    this->liftReg.field.liftReg_SEQU6_H = 0;
    this->liftReg.field.liftReg_SEQU7_L = 0;
    this->liftReg.field.liftReg_SEQU7_H = 0;
    this->liftReg.field.liftReg_SEQU8_L = 0;
    this->liftReg.field.liftReg_SEQU8_H = 0;
    uint32_t liftPulseDist = Register::calcLiftPulse(this->machineSettings.machineParam.liftGearRatio,
                                                     this->indexerLiftSettings.liftParam.distance);
    this->liftReg.field.liftReg_DIST_PULSE_L = ((uint16_t)(liftPulseDist&0x0000FFFF));
    this->liftReg.field.liftReg_DIST_PULSE_H = ((uint16_t)((liftPulseDist>>16)&0x0000FFFF));
    this->liftReg.field.REG_TEMP_UNIT = 0;
    this->liftReg.field.REG_GET_ZERO_OFF_CONTACT = 0;
    this->liftReg.field.REG_SKIPC_H = 0;
    this->liftReg.field.REG_SKIPC_L = 0;
    this->liftReg.field.liftReg_DOWN_DELAY = iLSett.liftParam.delayDown;
    this->liftReg.field.REG_PCB35_STAN = 0;
    this->liftReg.field.REG_PCB35_ROZ = 0;
    this->liftReg.field.REG_PCB35_SELECT = 0;
    this->liftReg.field.REG_PCB35_HEAT = 0;
    this->liftReg.field.REG_PCB35_ERR_DEV = 0;
    this->liftReg.field.REG_PCB35_MACHINE_TYPE = 0;
    this->liftReg.field.REG_PCB35_ERR_MESSAGE = 0;
}

uint32_t Register::calcLiftPulse(uint16_t gearRatio, uint16_t liftDist)
{
    double R = 90./25.4;
    double P = 15./25.4;
    double PULSE = 10000;
    double a, b, cosa, sina, sinb, r1;
    r1 = ((double)liftDist/100.) + (75./25.4);
    cosa = (R * R + P * P - r1 * r1) / (2 * R * P);
    a = acos(cosa);
    sina = sin(a);
    sinb = (R * sina)/r1;
    b = asin(sinb);
    if(r1 > (90./25.4))
        b = 3.1415926 - b;
    return static_cast<uint32_t>((b*PULSE*gearRatio)/(2. * 3.1415926));
}


