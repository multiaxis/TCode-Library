// TCode-Parser-Class-CPP v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 26/06/2022
// Please copy, share, learn, innovate, give attribution.
#include "TParser.h"
#include "utils/TString.h"

using namespace std;

namespace TCode::TParser {

bool isExtention(const char value)
{
    switch (toupper(value))
    {
    case 'I':
    case 'S':
        return true;
    default:
        return false;
    }
}

bool isRamp(const char value)
{
    switch (value)
    {
    case '<':
    case '>':
    case '=':
        return true;
    default:
        return false;
    }
}

bool isAxisIdValid(const AxisId &id)
{
    if (id.channel > 9 || id.channel < 0)
        return false;

    switch (id.type)
    {
    case AxisType::Auxiliary:
    case AxisType::Linear:
    case AxisType::Rotation:
    case AxisType::Vibration:
        break;
    default:
        return false;
    }

    return true;
}

AxisId getAxisId(size_t &index, const char *buffer, const size_t length)
{
    char type = toupper(TString::getCharAtOrDefault(index++, buffer, length));
    uint8_t channel = static_cast<uint8_t>(toupper(TString::getCharAtOrDefault(index++, buffer, length)) - '0');

    switch (type)
    {
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

CommandType getCommandType(const char *buffer, const size_t length)
{
    switch (toupper(TString::getCharAtOrDefault(0, buffer, length)))
    {
    case 'L':
    case 'R':
    case 'V':
    case 'A':
        return CommandType::Axis;
    case 'D':
        return CommandType::Device;
    case '$':
        return CommandType::Setup;
    default:
        return CommandType::None;
    }
}

size_t getNextCommand(deque<char> &inputBuffer, char *buffer, const size_t length)
{
    size_t index = 0;
    bool blevel = false; //TODO: ????
    while (!inputBuffer.empty() && (index < length - 1))
    {
        char charValue = inputBuffer.front();
        if(charValue == '\"')
            blevel = !blevel;

        if ((charValue == ' ') && !blevel)
        {
            inputBuffer.pop_front();
            break;
        }

        if (charValue == '\n')
        {
            inputBuffer.pop_front();
            break;
        } 

        buffer[index++] = inputBuffer.front();
        inputBuffer.pop_front();
    }
    
    //TODO: buffer[index] = '\0' ????
    return index;
}

bool parseAxisCommand(const char *buffer, const size_t length, AxisCommand &out)
{
    if (getCommandType(buffer, length) != CommandType::Axis)
        return false;

    size_t index = 0;
    AxisId id = getAxisId(index, buffer, length);
    if (!isAxisIdValid(id))
        return false;

    AxisExtentionType extentionType = AxisExtentionType::None;
    AxisRampType rampType = AxisRampType::None;
    AxisRampData rampIn = {};
    AxisRampData rampOut = {};
    float commandValue = 0;
    unsigned long commandExtention = 0;

    size_t logValue;
    if (!TString::getNextTCodeFloat(index, buffer, length, commandValue, logValue))
        return false;

    while (true)
    {
        if (isExtention(TString::getCharAtOrDefault(index, buffer, length)))
        {
            if (extentionType != AxisExtentionType::None)
                return false;

            if (!parseAxisExtention(index, buffer, length, extentionType, commandExtention))
                return false;
        }
        else if (isRamp(TString::getCharAtOrDefault(index, buffer, length)))
        {            
            if (!parseAxisRamp(index, buffer, length, rampType, rampIn, rampOut))
                return false;
        }
        else
        {
            break;
        }
    }

    // if the command has been processed and there are still characters left over the command has not been processed correctly/the command is incorrect
    if (toupper(TString::getCharAtOrDefault(index, buffer, length)) != '\0')
        return false;

    AxisData data { 
        .commandValue = commandValue,
        .commandExtention = commandExtention,
        .extentionType = extentionType,
        .rampType = rampType,
        .rampIn = rampIn,
        .rampOut = rampOut
    };

    out.data = data;
    out.id = id;
    return true;
}

AxisExtentionType getExtentionType(size_t &index, const char *buffer, const size_t length)
{
    switch (toupper(TString::getCharAtOrDefault(index++, buffer, length)))
    {
    case 'I':
        return AxisExtentionType::Time;
    case 'S':
        return AxisExtentionType::Speed;
    default:
        return AxisExtentionType::None;
    }
}

bool parseAxisExtention(size_t &index, const char *buffer, const size_t length, AxisExtentionType &extentionType, unsigned long &commandExtention)
{
    extentionType = getExtentionType(index, buffer, length);

    size_t logValue;
    if (!TString::getNextInt(index, buffer, length, commandExtention, logValue))
        return false;

    return true;
}

AxisRampType getRampType(size_t &index, const char *buffer, const size_t length)
{
    char first = TString::getCharAtOrDefault(index++, buffer, length);

    switch (first)
    {
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

bool parseAxisRamp(size_t &index, const char *buffer, const size_t length, AxisRampType &rampType, AxisRampData &rampIn, AxisRampData &rampOut)
{
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

bool parseAxisRampData(size_t &index, const char *buffer, const size_t length, const AxisRampType rampType, AxisRampData &data)
{
    data = {
        .tangent = 0,
        .hasTangent = true, 
        .weight = 1 / 3.0f,
        .hasWeight = false, 
        .autoTangent = rampType != AxisRampType::InOut
    };

    if (!isdigit(TString::getCharAtOrDefault(index, buffer, length)))
        return true;

    size_t logValue;
    float tangent;
    if (!TString::getNextTCodeFloat(index, buffer, length, tangent, logValue))
        return false;
    
    data.tangent = map(tangent, 0, 1, -0.999f, 0.999f);
    if (TString::getCharAtOrDefault(index, buffer, length) != '.')
        return true;
    
    index++;
    if (!isdigit(TString::getCharAtOrDefault(index, buffer, length)))
        return false;
    
    float weight;
    if (!TString::getNextTCodeFloat(index, buffer, length, weight, logValue))
        return false; 

    data.weight = constrain(weight, 0, 0.999f);
    data.hasWeight = true;
    return true;
}

bool parseSetupCommand(const char *buffer, const size_t length, SetupCommand &out)
{
    if (getCommandType(buffer, length) != CommandType::Setup)
        return false;

    size_t index = 1;
    AxisId id = getAxisId(index, buffer, length);
    if (!isAxisIdValid(id))
        return false;

    if (toupper(TString::getCharAtOrDefault(index++, buffer, length)) != '-')
        return false;

    unsigned long minValueLong; 
    size_t minValueLog;
    if (!TString::getNextInt(index, buffer, length, minValueLong, minValueLog)) 
        return false;

    if (toupper(TString::getCharAtOrDefault(index++, buffer, length)) != '-')
        return false;

    unsigned long maxValueLong; 
    size_t maxValueLog;
    if (!TString::getNextInt(index, buffer, length, maxValueLong, maxValueLog))
        return false;

    if ((toupper(TString::getCharAtOrDefault(index, buffer, length)) != '\0'))
        return false;

    float minValue = (double)minValueLong / pow10f(minValueLog);
    float maxValue = (double)maxValueLong / pow10f(maxValueLog);

    if (minValue > maxValue)
        return false;

    out.id = id;
    out.saveEntryData.min = minValue;
    out.saveEntryData.max = maxValue;
    out.saveEntryData.minLog = minValueLog;
    out.saveEntryData.maxLog = maxValueLog;
    return true;
}

bool parseDeviceCommand(const char *buffer, const size_t length, DeviceCommand &out)
{
    if (getCommandType(buffer, length) != CommandType::Device)
        return false;
        
    size_t index = 1;
    switch (toupper(TString::getCharAtOrDefault(index, buffer, length)))
    {
    case 'S':
        out = { DeviceCommandType::StopDevice };
        return true;
    case '0':
        out = { DeviceCommandType::GetSoftwareVersion };
        return true;
    case '1':
        out = { DeviceCommandType::GetTCodeVersion }; 
        return true;
    case '2':
        out = { DeviceCommandType::GetAssignedAxisValues };
        return true;
    default:
        out = { DeviceCommandType::None };
        return false;
    }
}

}