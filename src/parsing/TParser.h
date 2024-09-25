// TCode-Parser-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 26/06/2022
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include "../datatypes/CommandDataTypes.h"
#include "../datatypes/CommonDataTypes.h"
#include "../datatypes/EnumTypes.h"
#include <deque>

namespace TCode::TParser {

//#define TCODE_PARSER_TAG "TCodeParser"
    inline bool isExtention(const char value);

    inline bool isRamp(const char value);

    inline bool isHex(const char value);

    Datatypes::AxisExtentionType getExtentionType(size_t &index, const char *buffer, const size_t length);

    Datatypes::AxisRampType getRampType(size_t &index, const char *buffer, const size_t length);

    Datatypes::AxisId getAxisId(size_t &index, const char *buffer, const size_t length);

    Datatypes::AxisId getAxisIdFromStart(const char *buffer);

    Datatypes::CommandType getCommandType(const char *buffer, const size_t length);

    bool parseAxisCommand(const char *buffer, const size_t length, Datatypes::AxisCommandEvent &out);

    bool parseAxisExtention(size_t &index, const char *buffer, const size_t length, Datatypes::AxisExtentionType &extentionType, unsigned long &commandExtention);

    bool parseAxisRamp(size_t &index, const char *buffer, const size_t length, Datatypes::AxisRampData &rampOut);

    bool parseDeviceCommand(const char *buffer, const size_t length, Datatypes::DeviceCommandEvent &out);

    bool parseSetupCommand(const char *buffer, const size_t length, Datatypes::SetupCommandEvent &out);

    bool parseFirmwareCommand(const char *buffer, const size_t length, Datatypes::FirmwareCommandEvent &out);

    bool parseCommand(const char *buffer, const size_t length, Datatypes::TCodeEvent &out);

}