#ifndef HEADSETTING_H
#define HEADSETTING_H

#include <QByteArray>

class HeadSetting{
public:

    typedef enum HeadType_{
        PrintHead = 0,
        QuartzHead,
        InfraRedHead
    }HeadType;

    typedef struct HeadParameters_{
        HeadType headType = PrintHead;
        bool powerOn = true;
        uint16_t speedRear = 5;
        uint16_t speedFront = 50;
        uint16_t stroksCount = 1;
        uint16_t limitRear = 0;
        uint16_t limitFront = 30;
        uint16_t heatTime1 = 1;
        uint16_t heatTime2 = 1;
        uint16_t heatPower = 1;

        QByteArray toByteArray();
    }HeadParameters;

    typedef enum HeadCommandsEn_{
        Idle = 0,
        MoveRear,
        MoveFront,
        MoveTest,
        FL,
        SQ_FL,
        SQ,
        MPT_Move,
        Hold_on
    }HeadCommandsEn;

    typedef struct HeadComands_{
        HeadCommandsEn headCmd = Idle;

    }HeadComands;

    void fromByteArray(QByteArray hParamArr);

    HeadParameters operator =(HeadParameters hParam);

    HeadParameters headParam;

    HeadSetting(HeadParameters hParam);
    HeadSetting();
    ~HeadSetting();
};

class MachineSettings
{
public:
    typedef struct MachineParameters_{
        uint16_t HeadCount = 0;
        uint16_t WarningTime = 0;

        QByteArray toByteArray();
    }MachineParameters;

    MachineParameters machineParam;

    MachineSettings(MachineParameters mParam);
    MachineSettings();

    void fromByteArray(QByteArray headParamArray);

    ~MachineSettings();
};

class IndexerLiftSettings
{
public:
    typedef struct LiftParameters_{
        uint16_t distance = 0;
        int16_t homeOffcet = 0;
        uint16_t speed = 0;
        uint16_t acceleration = 0;
        uint16_t delayDown = 0;
        uint16_t delayUp = 0;

        QByteArray toByteArray();
    }LiftParameters;


    typedef struct IndexParameters_{
        uint16_t distance = 0;
        int16_t homeOffset = 0;
        int16_t distOffcet = 0;
        uint16_t speed = 0;
        uint16_t acceleration = 0;
        uint16_t speedRet = 0;
        uint16_t accelerationRet = 0;

        QByteArray toByteArray();
    }IndexParameters;

    IndexParameters indexerParam;
    LiftParameters liftParam;

    IndexerLiftSettings(IndexParameters indParam, LiftParameters lifParam);
    IndexerLiftSettings();

    void fromByteArray(QByteArray indParamArr, QByteArray lifParamArr);
};


#endif // HEADSETTING_H
