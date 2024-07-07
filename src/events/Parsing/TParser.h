// TCode-Parser-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 26/06/2022
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include <deque>
#include "../../datatypes/CommandDataTypes.h"
#include "../../datatypes/CommonDataTypes.h"
#include "../../datatypes/EnumTypes.h"

namespace TCode::TParser {

bool isExtention(const char value);

bool isRamp(const char value);

AxisExtentionType getExtentionType(size_t &index, const char *buffer, const size_t length);

AxisRampType getRampType(size_t &index, const char *buffer, const size_t length);

AxisId getAxisId(size_t &index, const char *buffer, const size_t length);

CommandType getCommandType(const char *buffer, const size_t length);

bool parseAxisCommand(const char *buffer, const size_t length, AxisCommandEvent &out);

bool parseAxisExtention(size_t &index, const char *buffer, const size_t length, AxisExtentionType &extentionType, unsigned long &commandExtention);

bool parseAxisRamp(size_t &index, const char *buffer, const size_t length, AxisRampType &rampType, AxisRampData &rampIn, AxisRampData &rampOut);

bool parseAxisRampData(size_t &index, const char *buffer, const size_t length, const AxisRampType rampType, AxisRampData &data);

bool parseDeviceCommand(const char *buffer, const size_t length, DeviceCommandEvent &out);

bool parseSetupCommand(const char *buffer, const size_t length, SetupCommandEvent &out);

bool parseCommand(const char* buffer, const size_t length, TCodeEvent &out);

}