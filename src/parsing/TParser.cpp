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
            return true;
        default:
            return false;
        }
    }


    Datatypes::AxisId getAxisIdFromStart(const char *buffer) {
        if (strlen(buffer) != 2)
            return {Datatypes::AxisType::None, UINT8_MAX};

        char type = toupper(TString::readCharOrDefault(0, buffer, 2));
        uint8_t channel = static_cast<uint8_t>(TString::readCharOrDefault(1, buffer, 2) - '0');
        //LogHandler::info(TCODE_PARSER_TAG, "Axis Channel Number:%d", channel);
        switch (type) {
        case 'L':
            return {Datatypes::AxisType::Linear, channel};
            break;
        case 'R':
            return {Datatypes::AxisType::Rotation, channel};
            break;
        case 'V':
            return {Datatypes::AxisType::Vibration, channel};
            break;
        case 'A':
            return {Datatypes::AxisType::Auxiliary, channel};
            break;
        default:
            return {Datatypes::AxisType::None, UINT8_MAX};
        }
    }

    Datatypes::AxisId getAxisId(size_t &index, const char *buffer, const size_t length) {
        char type = toupper(TString::readCharOrDefault(index++, buffer, length));
        uint8_t channel = static_cast<uint8_t>(TString::readCharOrDefault(index++, buffer, length) - '0');
        //LogHandler::info(TCODE_PARSER_TAG, "Axis Channel Number:%d", channel);
        switch (type) {
        case 'L':
            return {Datatypes::AxisType::Linear, channel};
            break;
        case 'R':
            return {Datatypes::AxisType::Rotation, channel};
            break;
        case 'V':
            return {Datatypes::AxisType::Vibration, channel};
            break;
        case 'A':
            return {Datatypes::AxisType::Auxiliary, channel};
            break;
        default:
            return {Datatypes::AxisType::None, UINT8_MAX};
        }
    }

    Datatypes::CommandType getCommandType(const char *buffer, const size_t length) {
        switch (toupper(TString::readCharOrDefault(0, buffer, length))) {
        case 'L':
        case 'R':
        case 'V':
        case 'A':
            return Datatypes::CommandType::Axis;
        case 'D':
            return Datatypes::CommandType::Device;
        case '$':
            return Datatypes::CommandType::Setup;
        case '*':
            return Datatypes::CommandType::Firmware;
        default:
            return Datatypes::CommandType::None;
        }
    }

    bool parseAxisCommand(const char *buffer, const size_t length, Datatypes::AxisCommandEvent &out) {
        if (getCommandType(buffer, length) != Datatypes::CommandType::Axis)
            return false;

        size_t index = 0;
        Datatypes::AxisId id = getAxisId(index, buffer, length);
        if (!id.isValid()) {
            //LogHandler::error(TCODE_PARSER_TAG, "Axis ID Invalid in Command, got axis type:\"%s\" channel number : \"%d\"", TString::axisTypeToVerboseString(id.type), id.channel);
            return false;
        }

        Datatypes::AxisRampType rampType = Datatypes::AxisRampType::None;
        Datatypes::AxisExtentionType extentionType = Datatypes::AxisExtentionType::None;
        Datatypes::AxisRampData rampIn = {.hasTangent = false, .hasWeight = false};
        Datatypes::AxisRampData rampOut = {.hasTangent = false, .hasWeight = false};
        float commandValue = 0;
        unsigned long commandExtention = 0;

        size_t logValue;
        if (!TString::readTCodeFloat(index, buffer, length, commandValue, logValue)) {
            //LogHandler::error(TCODE_PARSER_TAG, "Could not read value in axis command");
            return false;
        }

        bool hasInRamp = false;
        bool hasOutRamp = false;
        while (true) {
            char currentChar = TString::readCharOrDefault(index, buffer, length);
            if (isExtention(currentChar)) {
                if (extentionType != Datatypes::AxisExtentionType::None) {
                    //LogHandler::error(TCODE_PARSER_TAG, "Cannot have more than one extention per command");
                    return false;
                }

                if (!parseAxisExtention(index, buffer, length, extentionType, commandExtention)) {
                    //LogHandler::error(TCODE_PARSER_TAG, "Could not parse Axis extention");
                    return false;
                }
            } else if (isRamp(currentChar)) {
                if(hasInRamp && hasOutRamp)
                    return false;

                if(currentChar == '<' && !hasInRamp)
                {
                    parseAxisRamp(index, buffer, length, rampIn);
                    hasInRamp = true;
                }
                else if(currentChar == '>' && !hasOutRamp)
                {
                    parseAxisRamp(index, buffer, length, rampOut);
                    hasOutRamp = true;
                }
            } else {
                break;
            }
        }

        

        if(!(hasInRamp && hasOutRamp))
        {
            rampIn.autoTangent = true;
            rampOut.autoTangent = true;
        }

        // if the command has been processed and there are still characters left over the command has not been processed correctly/the command is incorrect
        if (toupper(TString::readCharOrDefault(index, buffer, length)) != '\0') {
            //LogHandler::error(TCODE_PARSER_TAG, "Could not parse chars left in buffer after parsing");
            return false;
        }

        Datatypes::AxisData data{
            .commandValue = commandValue,
            .commandExtention = commandExtention,
            .extentionType = extentionType,
            .rampIn = rampIn,
            .rampOut = rampOut};

        out.commandType = Datatypes::CommandType::Axis;
        out.data = data;
        out.id = id;
        return true;
    }

    Datatypes::AxisExtentionType getExtentionType(size_t &index, const char *buffer, const size_t length) {
        switch (toupper(TString::readCharOrDefault(index++, buffer, length))) {
        case 'I':
            return Datatypes::AxisExtentionType::Time;
        case 'S':
            return Datatypes::AxisExtentionType::Speed;
        default:
            return Datatypes::AxisExtentionType::None;
        }
    }

    bool parseAxisExtention(size_t &index, const char *buffer, const size_t length, Datatypes::AxisExtentionType &extentionType, unsigned long &commandExtention) {
        extentionType = getExtentionType(index, buffer, length);
        size_t logValue;
        if (!TString::readInt(index, buffer, length, commandExtention, logValue))
            return false;

        return true;
    }

    Datatypes::AxisRampType getRampType(size_t &index, const char *buffer, const size_t length) {
        char first = TString::readCharOrDefault(index++, buffer, length);

        switch (first) {
        case '<':
            return Datatypes::AxisRampType::In;
        case '>':
            return Datatypes::AxisRampType::Out;
        default:
            return Datatypes::AxisRampType::None;
        }
    }

    bool parseAxisRamp(size_t &index, const char *buffer, const size_t length, Datatypes::AxisRampData &rampOut) {
        //(<|>)(<tangent>(.<weight>))
        Datatypes::AxisRampType currentRampType = getRampType(index, buffer, length);
        if (currentRampType == Datatypes::AxisRampType::None)
            return false;

        Datatypes::AxisRampData data = {
            .tangent = 0.5,
            .weight = 1 / 3.0f,
            .hasTangent = false,
            .hasWeight = false,
            .autoTangent = false
        };

        size_t logValue = 0;
        float tangent = 0.5;
        float weight = 1 / 3.0f;
        
        if (!isdigit(TString::readCharOrDefault(index, buffer, length))) {
            return false;
        }

        if (!TString::readTCodeFloat(index, buffer, length, tangent, logValue))
            return false;

        data.tangent = constrain(TMath::mapf(tangent,0.0,1.0,-TMath::doubleLimit,TMath::doubleLimit), -TMath::doubleLimit, TMath::doubleLimit);
        data.hasTangent = true;

        if (TString::readCharOrDefault(index, buffer, length) == '.')
        {
            index++;
            if (!isdigit(TString::readCharOrDefault(index++, buffer, length)))
                return false;

            
            if (!TString::readTCodeFloat(index, buffer, length, weight, logValue))
                return false;
            data.weight = constrain(weight, 0, TMath::doubleLimit);
            data.hasWeight = true;
        }

        rampOut = data;
        return true;
    }

    bool parseSetupCommand(const char *buffer, const size_t length, Datatypes::SetupCommandEvent &out) {
        if (getCommandType(buffer, length) != Datatypes::CommandType::Setup)
            return false;

        size_t index = 1;
        Datatypes::AxisId id = getAxisId(index, buffer, length);
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
        out.commandType = Datatypes::CommandType::Setup;
        return true;
    }

    bool parseFirmwareCommand(const char *buffer, const size_t length, Datatypes::FirmwareCommandEvent &out) {
        size_t index = 1;
        if(!TString::readVIntHex(index,buffer,length,out.firmwareID))
        {
            //LogHandler::error(TCODE_PARSER_TAG, "Invalid Firmware ID in Command:\"%s\"", buffer);
            return false;
        }

        for(size_t i = 0; i < sizeof(out.value)-1; i++)
            out.value[i] = '\0';

        for(size_t i = index; i < length; i++)
        {
            size_t offset = i - index;
            if(offset < (sizeof(out.value)-1))
                out.value[offset] = buffer[i];
            else
                break;
        }

        out.value[sizeof(out.value)-1] = '\0';
        out.commandType = Datatypes::CommandType::Firmware;
        return true;
    }

    bool parseCommand(const char *buffer, const size_t length, Datatypes::TCodeEvent &out) {
        Datatypes::CommandType type = TParser::getCommandType(buffer, length);

        //LogHandler::info(TCODE_PARSER_TAG, "Parsing Command Buffer:\"%s\"", buffer);

        switch (type) {
        case Datatypes::CommandType::Axis: {
            //LogHandler::info(TCODE_PARSER_TAG, "Command Type : Axis");
            Datatypes::AxisCommandEvent result;
            if (TParser::parseAxisCommand(buffer, length, result)) {
                result.commandType = Datatypes::CommandType::Axis;
                out.axisCommand = result;
                //LogHandler::info(TCODE_PARSER_TAG, "Value: %f", result.data.commandValue);
                //LogHandler::info(TCODE_PARSER_TAG, "Extention Type:%d", (int)result.data.extentionType);
                //LogHandler::info(TCODE_PARSER_TAG, "Extention: %d", result.data.commandExtention);
                return true;
            }
            break;
        }
        case Datatypes::CommandType::Device: {
            //LogHandler::info(TCODE_PARSER_TAG, "Command Type : Device");
            Datatypes::DeviceCommandEvent result;
            if (TParser::parseDeviceCommand(buffer, length, result)) {
                result.commandType = Datatypes::CommandType::Device;
                out.deviceCommand = result;
                return true;
            }
            break;
        }
        case Datatypes::CommandType::Setup: {
            //LogHandler::info(TCODE_PARSER_TAG, "Command Type : Setup");
            Datatypes::SetupCommandEvent result;
            if (TParser::parseSetupCommand(buffer, length, result)) {
                result.commandType = Datatypes::CommandType::Setup;
                out.setupCommand = result;
                return true;
            }
            break;
        }
        case Datatypes::CommandType::Firmware: {
            //LogHandler::info(TCODE_PARSER_TAG, "Command Type : Firmware");
            Datatypes::FirmwareCommandEvent result;
            if (TParser::parseFirmwareCommand(buffer, length, result)) {
                result.commandType = Datatypes::CommandType::Firmware;
                out.firmwareCommand = result;
                return true;
            }
            break;
        }
        default:
            //LogHandler::info(TCODE_PARSER_TAG, "Command Type : Unknown");
            out.commandType = Datatypes::CommandType::None;
            break;
        }
        return false;
    }

    bool parseDeviceCommand(const char *buffer, const size_t length, Datatypes::DeviceCommandEvent &out) {
        if (getCommandType(buffer, length) != Datatypes::CommandType::Device)
            return false;

        size_t index = 1;
        Datatypes::DeviceCommandType command = Datatypes::DeviceCommandType::None;
        switch (toupper(TString::readCharOrDefault(index, buffer, length))) {
        case 'S': command = Datatypes::DeviceCommandType::StopDevice; break;
        case '0': command = Datatypes::DeviceCommandType::GetSoftwareVersion; break;
        case '1': command = Datatypes::DeviceCommandType::GetTCodeVersion; break;
        case '2': command = Datatypes::DeviceCommandType::GetAssignedAxisValues; break;
        default: command = Datatypes::DeviceCommandType::None; break;
        }

        out = {Datatypes::CommandType::Device, command};
        return command != Datatypes::DeviceCommandType::None;
    }

}