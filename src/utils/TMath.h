// TCode-Floating-Point-functions-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 29/11/2023
// Please copy, share, learn, innovate, give attribution.
//
#pragma once

#include <Arduino.h>
#include "../../common/datatypes/CommonDataTypes.h"
#include "../../common/datatypes/EnumTypes.h"

namespace TCode::TMath {

float mapf(float x, float in_min, float in_max, float out_min, float out_max);

float interpolate(float x, float x0, float y0, AxisRampData r0, float x1, float y1, AxisRampData r1);

unsigned long getTCodeFromFloat(float value, int precision, uint8_t &logOut);

float getFloatFromTCode(unsigned long value, int precision);

unsigned long getTCodeEstimateLogFromFloat(float value);

}
