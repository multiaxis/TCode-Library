// TCode-Enums-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 05/02/2022
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include <Arduino.h>
#include "../common/DataTypes.h"

namespace TCode {

enum class CommandType {
    Axis,
    Device,
    Setup,
    None,
};

enum class DeviceCommandType {
    GetSoftwareVersion,
    GetTCodeVersion,
    GetAssignedAxisValues,
    StopDevice,
    None,
};

struct SaveEntry {
    float min;
    float max;
    uint8_t minLog;
    uint8_t maxLog;
};

struct AxisState {
    unsigned long startTime;
    unsigned long endTime;
    float startPosition;
    float endPosition;
    AxisRampData startRamp;
    AxisRampData endRamp;
    AxisData data;
};

struct DeviceCommand {
    DeviceCommandType type;
};

struct AxisCommand {
    AxisId id;
    AxisData data;
};

struct SetupCommand {
    AxisId id;
    SaveEntry saveEntryData;
};

}
