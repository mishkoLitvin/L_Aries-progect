#ifndef HEADSETTING_H
#define HEADSETTING_H

#include <QByteArray>

typedef u_int16_t uint16_t;
typedef u_int8_t uint8_t;

class HeadSetting{
public:
    enum{
        HeadDeviceAdrOffcet = 0x0002
    };

    enum {
        HeadHeadType = 0x21,
        HeadPowerOn,
        HeadSpeedRear = 0x02,
        HeadSpeedFront = 0x03,
        HeadStroksCount = 0x07,
        HeadLimitRear = 0x16,
        HeadLimitFront = 0x17,
        HeadHeatTime1Q = 0x07,
        HeadHeatTime2Q = 0x08,
        HeadHeatPower = 0x1D,
        HeadHeatTime1IR = 0x1F,
        HeadHeatTime2IR = 0x20,
        HeadHeatDryRange
    };

    typedef enum HeadType_{
        PrintHead = 0,
        QuartzHead,
        InfraRedHead
    }HeadType;

    typedef struct HeadParameters_{
        HeadType headType;
        bool powerOn;
        uint16_t speedRear;
        uint16_t speedFront;
        uint16_t stroksCount;
        uint16_t limitRear;
        uint16_t limitFront;
        uint16_t heatTime1;
        uint16_t heatTime2;
        uint16_t heatPower;

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
        HeadCommandsEn headCmd;

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

    enum{
        MasterDevice = 0x0000
    };

    enum{
        MasterHeadCount = 0x0001,
        MasterIndexLiftCommand = 0x0004
    };

    typedef union MachineState_{
        struct{
            uint8_t printStop:1;    //0
            uint8_t reserved:1;     //1
            uint8_t lockUnLock:1;   //2
            uint8_t upDown:1;       //3
            uint8_t halfFull:1;     //4
            uint8_t reserved2:1;    //5
            uint8_t manualAuto:1;   //6
            uint8_t airRelease:1;   //7
            uint8_t easySetup:1;    //8
            uint8_t reserved3:7;    //9-15
        } bit;
        uint16_t all;
    }MachineState;

    typedef enum MachineType_{
        VoltServo = 0x0011,
        VoltAC = 0x0021,
        Vector = 0x0012,
        Titan = 0x0013
    }MachineType;

    typedef struct MachineParameters_{
        uint16_t headCount;
        uint16_t warningTime;
        int16_t direction;
        MachineType machineType;
        QByteArray toByteArray();
    }MachineParameters;

    MachineParameters machineParam;

    MachineSettings(MachineParameters mParam);
    MachineSettings();

    void fromByteArray(QByteArray machineParamArray);

    static bool serviceWidgetsEn;

public:
    static bool getServiceWidgEn();
    static void setServiceWidgEn(bool servEn);

};

class IndexerLiftSettings
{
public:

    enum{
        LiftDevice = 0x0001,
        IndexerDevice = 0x0002
    };

    enum{
        IndexHomeOffset = 0x01,
        IndexDistOffcet = 0x02,
        IndexSpeed = 0x03,
        IndexDirection = 0x04,
        LiftDelayDown = 0x07,
        LiftDelayUp = 0x07,
        IndexDistance = 0x08,
        IndexAcceleration = 0x0A,
        IndexAccelerationRet = 0x0B,
        IndexSpeedRet = 0x0C,
        LiftHomeOffcet = 0x15,
        LiftDistance = 0x16,
        LiftSpeed = 0x17,
        LiftAcceleration = 0x18,
    };

    typedef struct LiftParameters_{
        uint16_t distance;
        int16_t homeOffcet;
        uint16_t speed;
        uint16_t acceleration;
        uint16_t delayDown;
        uint16_t delayUp;

        QByteArray toByteArray();
    }LiftParameters;


    typedef struct IndexParameters_{
        uint16_t distance;
        int16_t homeOffset;
        int16_t distOffcet;
        uint16_t speed;
        uint16_t acceleration;
        uint16_t speedRet;
        uint16_t accelerationRet;

        QByteArray toByteArray();
    }IndexParameters;

    typedef enum IndexerCommandsEn_{
        IndexLock_UnLock = 0x02,
        MoveUp_Down = 0x03,
        MoveLeft,
        MoveRight,
        MoveLeftHalf,
        MoveRightHalf,
        MoveFull_Half = 0x04,
        Auto_Manual = 0x06,
        PrintStart_Stop = 0x00,
        AirRelease = 0x07,
        EasySetup = 0x08
    }IndexerCommandsEn;



    IndexParameters indexerParam;
    LiftParameters liftParam;

    IndexerLiftSettings(IndexParameters indParam, LiftParameters lifParam);
    IndexerLiftSettings();

    void fromByteArray(QByteArray indParamArr, QByteArray lifParamArr);
};


#endif // HEADSETTING_H
