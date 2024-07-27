// TCode-DataTypes-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 05/02/2022
// Please copy, share, learn, innovate, give attribution.
#pragma once
// #include <cstdint.h>
#include "CommonDataTypes.h"
#include "EnumTypes.h"
#include <Arduino.h>

namespace TCode {

    /**
     * @brief structure for save entry data
     */
    struct SaveEntry {
        uint8_t minLog;
        uint8_t maxLog;
        float min;
        float max;
    };

    struct DeviceCommandEvent {
        CommandType commandType;
        DeviceCommandType type;
    };

    struct AxisCommandEvent {
        CommandType commandType;
        AxisId id;
        AxisData data;
    };

    struct SetupCommandEvent {
        CommandType commandType;
        AxisId id;
        SaveEntry saveEntryData;
    };

    /**
     * @brief TCode Command Union
     */
    typedef union _TCodeEvent {
        CommandType commandType;
        DeviceCommandEvent deviceCommand;
        AxisCommandEvent axisCommand;
        SetupCommandEvent setupCommand;
    } TCodeEvent;

}