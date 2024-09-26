// TCode-Floating-Point-functions-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 29/11/2023
// Please copy, share, learn, innovate, give attribution.
//
#pragma once

#include <Arduino.h>

#include "../datatypes/CommonDataTypes.h"
#include "../datatypes/EnumTypes.h"

namespace TCode::TMath {
    static const double doubleLimit = (1.0-__DBL_EPSILON__);
    static const double error = 1e-6;

    float mapf(float x, float in_min, float in_max, float out_min, float out_max);

    float interpolate(float time, float x0, float y0, Datatypes::AxisRampData r0, float x1, float y1, Datatypes::AxisRampData r1);

    unsigned long getTCodeFromFloat(float value, int precision, uint8_t &logOut);

    float getFloatFromTCode(unsigned long value, int precision);

    unsigned long getTCodeEstimateLogFromFloat(float value);
}
