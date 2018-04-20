#ifndef HEADSETTING_H
#define HEADSETTING_H

#include <QByteArray>
#include <QStringList>

typedef u_int32_t uint32_t;
typedef u_int16_t uint16_t;
typedef u_int8_t uint8_t;


class HeadSetting{
public:
    enum{
        HeadDeviceAdrOffcet = 0x0002
    };

    enum {
        HeadSpeedRear = 0x02,
        HeadSpeedFront = 0x03,
        HeadFlDwellTime = 0x04,
        HeadSqDwellTime = 0x05,
        HeadStroksCount = 0x06,
        HeadFlashTime1Q = 0x07,
        HeadFlashTime2Q = 0x08,
        HeadSBStroksCount = 0x0E,
        HeadHoldOnOff = 0x14,
        HeadRangeLimit1 = 0x16,
        HeadRangeLimit2 = 0x17,
        HeadHeatTemper = 0x18,
        HeadHeatTimeIR = 0x19,
        HeadFlashPowerStBy = 0x1A,
        HeadFlashTimeStBy = 0x1B,
        HeadFlashPowerWtoutIR = 0x1D,
        HeadHeatWarmTime = 0x1F,
        HeadFlashWarmTime = 0x20,
        HeadHeadType = 0x21,
        HeadPowerOn,
        HeadHeatDryRange,
        HeadHeatTime1IR,
        HeadHeatTime2IR
    };

    typedef enum HeadType_{
        PrintHead = 0x0258,
        QuartzHead,
        InfraRedHead
    }HeadType;

    typedef struct HeadParameters_{
        HeadType headType;
        bool powerOn;
        uint16_t speedRear;
        uint16_t speedFront;
        uint16_t stroksCount;
        uint16_t stroksSBCount;
        uint16_t limitRear;
        uint16_t limitFront;
        uint16_t dwellFLTime;
        uint16_t dwellSQTime;
        uint16_t heatTime1;
        uint16_t heatTime2;
        uint16_t heatPower;
        uint16_t heatLimit;
        uint16_t heatTime1Q;
        uint16_t heatTime2Q;
        uint16_t dryPowerQ;
        uint16_t stepbackDryTimeQ;
        uint16_t temperatureSetQ;
        uint16_t dryTimeQ;
        uint16_t standbyTimeQ;
        uint16_t standbyPowerQ;
        uint16_t warmFlashTimeQ;

        QByteArray toByteArray();
    }HeadParameters;

    typedef enum HeadCommandsEn_{
        Idle = 0,
        MoveRear = 0x0672,
        MoveFront,
        MoveTest,
        SQ,
        FL,
        SQ_FL,
        Hold_Off,
        Hold_On,
        TeachPosition1,
        TeachPosition2,
        SQ_FL_UpDown,
        MPT_Move,
        PressureSQ = 0x005A,
        AirRelease = 0x005A,
        TimeSelect_1 = 0x025B,
        TimeSelect_2,
        TimeSelect_3,
        Stepback,
        Plast_water,
        Preheat,
        IndexHere,
        SensorOn_Off,
        TemperatureUnit,
        WarmFlash,
        HeatTest
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

    static uint32_t headStateAll;

    static uint16_t getHeadStateLo();
    static uint16_t getHeadStateHi();
    static bool getHeadStateAtIndex(uint8_t index);
    static void setHeadStateAtIndex(uint8_t index, bool state);
};


class MachineSettings
{
public:

    enum Devices{
        MasterDevice = 0x0000
    };

    enum MasterDataPlaces{
        MasterHeadCount = 0x0001,
        MasterLastButton = 0x0003,
        MasterIndexLiftCommand = 0x0004,
        MasterHeadStateLo = 0x0005,
        MasterHeadStateHi = 0x0009,
        MasterMachineType = 0x0011,
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
        VoltServo = 0x0000,
        VoltAC,
        Vector,
        TitanASE,
        TitanASA,
        TitanAAA
    }MachineType;

    QStringList machineTypeList;
    QList<int> machineTypeData;

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
    static MachineType machineTypeStat;

public:
    static bool getServiceWidgEn();
    static void setServiceWidgEn(bool servEn);

    static MachineType getMachineType();
    static void setMachineType(MachineType mType);

};

class IndexerLiftSettings
{
public:

    enum Devices{
        LiftDevice = 0x0002,
        IndexerDevice = 0x0001
    };

    enum IndexerLiftDataPlaces{
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
        WarningTime = 0x0D,

        LiftHomeOffcet = 0x15,
        LiftDistance = 0x16,
        LiftSpeed = 0x17,
        LiftAcceleration = 0x18,
        LoadHeadState = 0x1F,

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
        Machine_Reset = 0x0001,
        Machine_Home = 0x0009,
        IndexLock = 0x000A,
        IndexUnLock = 0x000A,
        MoveUp_Down = 0x0006,
        MoveLeft = 0x0005,
        MoveRight = 0x0007,
        MoveLeftHalf = 0x0005,
        MoveRightHalf = 0x0007,
        MoveFull_Half = 0x0004,
        Auto_Manual = 0x0008,
        PrintStart = 0x000B,
        PrintStop = 0x000B,
        AirRelease = 0x005A,
        EasySetup = 0x08,
        IndexMoveHome = 0x00B8,
        IndexMoveEnd = 0x00B9,
        LiftMoveHome = 0x00B8,
        LiftMoveEnd = 0x00B9,
        IndexDirChange = 0x005D,

    }IndexerCommandsEn;



    IndexParameters indexerParam;
    LiftParameters liftParam;

    IndexerLiftSettings(IndexParameters indParam, LiftParameters lifParam);
    IndexerLiftSettings();

    void fromByteArray(QByteArray indParamArr, QByteArray lifParamArr);
};


#endif // HEADSETTING_H
