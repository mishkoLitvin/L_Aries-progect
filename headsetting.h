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

    HeadParameters headParameters;

    HeadSetting(HeadParameters hParam);
    HeadSetting();
    ~HeadSetting();

    typedef struct IndexerParameters_{
        uint16_t distance;
        int16_t homeOffcet;
        uint16_t speed;
        uint16_t acceleration;

        QByteArray toByteArray();
    }IndexerParameters;

};


#endif // HEADSETTING_H
