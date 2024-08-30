// TCode-Parser-Class-CPP v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 26/06/2022
// Please copy, share, learn, innovate, give attribution.
#include "TParser.h"
#include "../utils/TString.h"

namespace TCode::TParser {

    bool isExtention(const char value) {
        switch (toupper(value)) {
        case 'I':
        case 'S':
            return true;
        default:
            return false;
        }
    }

    bool isRamp(const char value) {
        switch (value) {
        case '<':
        case '>':
        case '=':
            return true;
        default:
            return false;
        }
    }


    AxisId getAxisIdFromStart(const char *buffer) {
        if (strlen(buffer) != 2)
            return {AxisType::None, UINT8_MAX};

        char type = toupper(TString::readCharOrDefault(0, buffer, 2));
        uint8_t channel = static_cast<uint8_t>(TString::readCharOrDefault(1, buffer, 2) - '0');
        LogHandler::info(TCODE_PARSER_TAG, "Axis Channel Number:%d", channel);
        switch (type) {
        case 'L':
            return {AxisType::Linear, channel};
            break;
        case 'R':
            return {AxisType::Rotation, channel};
            break;
        case 'V':
            return {AxisType::Vibration, channel};
            break;
        case 'A':
            return {AxisType::Auxiliary, channel};
            break;
        default:
            return {AxisType::None, UINT8_MAX};
        }
    }

    AxisId getAxisId(size_t &index, const char *buffer, const size_t length) {
        char type = toupper(TString::readCharOrDefault(index++, buffer, length));
        uint8_t channel = static_cast<uint8_t>(TString::readCharOrDefault(index++, buffer, length) - '0');
        LogHandler::info(TCODE_PARSER_TAG, "Axis Channel Number:%d", channel);
        switch (type) {
        case 'L':
            return {AxisType::Linear, channel};
            break;
        case 'R':
            return {AxisType::Rotation, channel};
            break;
        case 'V':
            return {AxisType::Vibration, channel};
            break;
        case 'A':
            return {AxisType::Auxiliary, channel};
            break;
        default:
            return {AxisType::None, UINT8_MAX};
        }
    }

    CommandType getCommandType(const char *buffer, const size_t length) {
        switch (toupper(TString::readCharOrDefault(0, buffer, length))) {
        case 'L':
        case 'R':
        case 'V':
        case 'A':
            return CommandType::Axis;
        case 'D':
            return CommandType::Device;
        case '$':
            return CommandType::Setup;
        case '*':
            return CommandType::Firmware;
        default:
            return CommandType::None;
        }
    }

    bool parseAxisCommand(const char *buffer, const size_t length, AxisCommandEvent &out) {
        if (getCommandType(buffer, length) != CommandType::Axis)
            return false;

        size_t index = 0;
        AxisId id = getAxisId(index, buffer, length);
        if (!id.isValid()) {
            LogHandler::error(TCODE_PARSER_TAG, "Axis ID Invalid in Command, got axis type:\"%s\" channel number : \"%d\"", TString::axisTypeToVerboseString(id.type), id.channel);
            return false;
        }

        AxisRampType rampType = AxisRampType::None;
        AxisExtentionType extentionType = AxisExtentionType::None;
        AxisRampData rampIn = {.hasTangent = false, .hasWeight = false};
        AxisRampData rampOut = {.hasTangent = false, .hasWeight = false};
        float commandValue = 0;
        unsigned long commandExtention = 0;

        size_t logValue;
        if (!TString::readTCodeFloat(index, buffer, length, commandValue, logValue)) {
            LogHandler::error(TCODE_PARSER_TAG, "Could not read value in axis command");
            return false;
        }

        while (true) {
            if (isExtention(TString::readCharOrDefault(index, buffer, length))) {
                if (extentionType != AxisExtentionType::None) {
                    LogHandler::error(TCODE_PARSER_TAG, "Cannot have more than one extention per command");
                    return false;
                }

                if (!parseAxisExtention(index, buffer, length, extentionType, commandExtention)) {
                    LogHandler::error(TCODE_PARSER_TAG, "Could not parse Axis extention");
                    return false;
                }
            } else if (isRamp(TString::readCharOrDefault(index, buffer, length))) {
                if (!parseAxisRamp(index, buffer, length, rampType, rampIn, rampOut)) {
                    LogHandler::error(TCODE_PARSER_TAG, "Could not parse Axis ramp");
                    return false;
                }
            } else {
                break;
            }
        }

        // if the command has been processed and there are still characters left over the command has not been processed correctly/the command is incorrect
        if (toupper(TString::readCharOrDefault(index, buffer, length)) != '\0') {
            LogHandler::error(TCODE_PARSER_TAG, "Could not parse chars left in buffer after parsing");
            return false;
        }

        AxisData data{
            .commandValue = commandValue,
            .commandExtention = commandExtention,
            .extentionType = extentionType,
            .rampIn = rampIn,
            .rampOut = rampOut};

        out.data = data;
        out.id = id;
        return true;
    }

    AxisExtentionType getExtentionType(size_t &index, const char *buffer, const size_t length) {
        switch (toupper(TString::readCharOrDefault(index++, buffer, length))) {
        case 'I':
            return AxisExtentionType::Time;
        case 'S':
            return AxisExtentionType::Speed;
        default:
            return AxisExtentionType::None;
        }
    }

    bool parseAxisExtention(size_t &index, const char *buffer, const size_t length, AxisExtentionType &extentionType, unsigned long &commandExtention) {
        extentionType = getExtentionType(index, buffer, length);

        size_t logValue;
        if (!TString::readInt(index, buffer, length, commandExtention, logValue))
            return false;

        return true;
    }

    AxisRampType getRampType(size_t &index, const char *buffer, const size_t length) {
        char first = TString::readCharOrDefault(index++, buffer, length);

        switch (first) {
        case '<':
            return AxisRampType::In;
        case '>':
            return AxisRampType::Out;
        case '=':
            return AxisRampType::InOut;
        default:
            return AxisRampType::None;
        }
    }

    bool parseAxisRamp(size_t &index, const char *buffer, const size_t length, AxisRampType &rampType, AxisRampData &rampIn, AxisRampData &rampOut) {
        //(<|>|=)(<tangent>(.<weight>))
        if (rampType == AxisRampType::InOut)
            return false;

        AxisRampType currentRampType = getRampType(index, buffer, length);
        if (currentRampType == AxisRampType::None)
            return false;
        if (currentRampType == rampType)
            return false;

        AxisRampData data;
        if (!parseAxisRampData(index, buffer, length, currentRampType, data))
            return false;

        if (currentRampType == AxisRampType::In || currentRampType == AxisRampType::InOut)
            rampIn = data;
        if (currentRampType == AxisRampType::Out || currentRampType == AxisRampType::InOut)
            rampOut = data;

        if (rampType != AxisRampType::None)
            rampType = AxisRampType::InOut;

        return true;
    }

    bool parseAxisRampData(size_t &index, const char *buffer, const size_t length, const AxisRampType rampType, AxisRampData &data) {
        data = {
            .tangent = 0,
            .weight = 1 / 3.0f,
            .hasTangent = false,
            .hasWeight = false,
            .autoTangent = rampType != AxisRampType::InOut};

        if (!isdigit(TString::readCharOrDefault(index++, buffer, length))) {
            return false;
        }

        size_t logValue;
        float tangent;
        if (!TString::readTCodeFloat(index, buffer, length, tangent, logValue))
            return false;

        data.tangent = TMath::mapf(tangent, 0.0f, 1.0f, -TMath::doubleLimit, TMath::doubleLimit);
        data.hasTangent = true;
        if (TString::readCharOrDefault(index, buffer, length) != '.')
            return true;
        index++;
        if (!isdigit(TString::readCharOrDefault(index++, buffer, length)))
            return false;

        float weight;
        if (!TString::readTCodeFloat(index, buffer, length, weight, logValue))
            return false;

        data.weight = constrain(weight, 0, TMath::doubleLimit);
        data.hasWeight = true;
        return true;
    }

    bool parseSetupCommand(const char *buffer, const size_t length, SetupCommandEvent &out) {
        if (getCommandType(buffer, length) != CommandType::Setup)
            return false;

        size_t index = 1;
        AxisId id = getAxisId(index, buffer, length);
        if (!id.isValid())
            return false;

        if (toupper(TString::readCharOrDefault(index++, buffer, length)) != '-')
            return false;

        unsigned long minValueLong;
        size_t minValueLog;
        if (!TString::readInt(index, buffer, length, minValueLong, minValueLog))
            return false;

        if (toupper(TString::readCharOrDefault(index++, buffer, length)) != '-')
            return false;

        unsigned long maxValueLong;
        size_t maxValueLog;
        if (!TString::readInt(index, buffer, length, maxValueLong, maxValueLog))
            return false;

        if ((toupper(TString::readCharOrDefault(index, buffer, length)) != '\0'))
            return false;

        float minValue = TMath::getFloatFromTCode(minValueLong, minValueLog);
        float maxValue = TMath::getFloatFromTCode(maxValueLong, maxValueLog);

        if (minValue > maxValue)
            return false;

        out.id = id;
        out.saveEntryData.min = minValue;
        out.saveEntryData.max = maxValue;
        out.saveEntryData.minLog = minValueLog;
        out.saveEntryData.maxLog = maxValueLog;
        return true;
    }

    bool parseFirmwareCommand(const char *buffer, const size_t length, FirmwareCommandEvent &out) {
        size_t index = 1;
        out.value = new String();
        if(!TString::readVIntHex(index,buffer,length,out.firmwareID))
            return false;

        for(size_t i = index; i < length; i++)
        {
            (*out.value)+=buffer[i];
        }
        
        return true;
    }

    bool parseCommand(const char *buffer, const size_t length, TCodeEvent &out) {
        CommandType type = TParser::getCommandType(buffer, length);

        LogHandler::info(TCODE_PARSER_TAG, "Parsing Command Buffer:\"%s\"", buffer);

        switch (type) {
        case CommandType::Axis: {
            LogHandler::info(TCODE_PARSER_TAG, "Command Type : Axis");
            AxisCommandEvent result;
            out.commandType = CommandType::Axis;
            if (TParser::parseAxisCommand(buffer, length, result)) {
                out.axisCommand = result;
                LogHandler::info(TCODE_PARSER_TAG, "Value: %f\nExtention Type:%d\nExtention: %d\n", result.data.commandValue, (int)result.data.extentionType, result.data.commandExtention);
                return true;
            }
            break;
        }
        case CommandType::Device: {
            LogHandler::info(TCODE_PARSER_TAG, "Command Type : Device");
            DeviceCommandEvent result;
            out.commandType = CommandType::Device;
            if (TParser::parseDeviceCommand(buffer, length, result)) {
                out.deviceCommand = result;
                return true;
            }
            break;
        }
        case CommandType::Setup: {
            LogHandler::info(TCODE_PARSER_TAG, "Command Type : Setup");
            SetupCommandEvent result;
            out.commandType = CommandType::Setup;
            if (TParser::parseSetupCommand(buffer, length, result)) {
                out.setupCommand = result;
                return true;
            }
            break;
        }
        case CommandType::Firmware: {
            LogHandler::info(TCODE_PARSER_TAG, "Command Type : Firmware");
            FirmwareCommandEvent result;
            out.commandType = CommandType::Firmware;
            if (TParser::parseFirmwareCommand(buffer, length, result)) {
                out.firmwareCommand = result;
                return true;
            }
            break;
        }
        default:
            LogHandler::info(TCODE_PARSER_TAG, "Command Type : Unknown");
            out.commandType = CommandType::None;
            break;
        }
        return false;
    }

    bool parseDeviceCommand(const char *buffer, const size_t length, DeviceCommandEvent &out) {
        if (getCommandType(buffer, length) != CommandType::Device)
            return false;

        size_t index = 1;
        switch (toupper(TString::readCharOrDefault(index, buffer, length))) {
        case 'S':
            out = {CommandType::Device, DeviceCommandType::StopDevice};
            return true;
        case '0':
            out = {CommandType::Device, DeviceCommandType::GetSoftwareVersion};
            return true;
        case '1':
            out = {CommandType::Device, DeviceCommandType::GetTCodeVersion};
            return true;
        case '2':
            out = {CommandType::Device, DeviceCommandType::GetAssignedAxisValues};
            return true;
        default:
            out = {CommandType::Device, DeviceCommandType::None};
            return false;
        }
    }

}