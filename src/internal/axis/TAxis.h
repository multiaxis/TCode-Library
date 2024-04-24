// TCode-Axis-Unbuffered-H v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 18/11/2023
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include "../DataTypes.h"

namespace TCode {

const int TCODE_MIN_AXIS_SMOOTH_INTERVAL = 3;
const int TCODE_MAX_AXIS_SMOOTH_INTERVAL = 100;

class TAxis {
public:
    TAxis() = delete;
    TAxis(const char *name, AxisId id, float defaultPosition);

    void set(const AxisData &data);
    float getPosition();
    void stop();
    bool changed();
    const char *getName();
    AxisId getId();
    unsigned long getLastCommandTime();

private:
    unsigned long lastCommandTime;
    const char *name;
    AxisId id;
    float lastPosition;
    float defaultPosition;
    int minInterval;
    AxisState currentState;
};

}
