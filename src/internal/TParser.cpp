// TCode-Parser-Class-CPP v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 26/06/2022
// Please copy, share, learn, innovate, give attribution.
#include "TParser.h"
#include "utils/TString.h"

using namespace std;
using namespace TCode;

bool TParser::idValid(const ChannelType type, const uint8_t channel)
{
    if (channel > 9 || channel < 0) // check if channel number is not out of range
        return false;

    switch (type) // check if the type is correct
    {
    case ChannelType::Auxiliary:
    case ChannelType::Linear:
    case ChannelType::Rotation:
    case ChannelType::Vibration:
        break;
    default:
        return false;
    }

    return true;
}

bool TParser::idValid(const ChannelID id)
{
    return idValid(id.type, id.channel);
}

int TParser::getStrfromID(char *buffer, const size_t length, const ChannelID &id)
{
    size_t index = 0;
    switch (id.type) // gets the Channel type char and appends to the output string
    {
    case ChannelType::Auxiliary:
        buffer[index] = 'A';
        break;
    case ChannelType::Linear:
        buffer[index] = 'L';
        break;
    case ChannelType::Rotation:
        buffer[index] = 'R';
        break;
    case ChannelType::Vibration:
        buffer[index] = 'V';
        break;
    default:
        buffer[index] = '?';
    }
    index++;
    index += TString::uintToStr(id.channel, buffer, length, index);

    return index;
}

void TParser::getStrfromID(const ChannelID &id, String &out)
{
    char buffer[10];
    for(int i = 0; i < 10; i++)
        buffer[i] = '\0';

    getStrfromID(buffer, 10, id);
    size_t index = 0;
    for (; index < 10; index++)
    {
        if (buffer[index] == '\0')
            break;
        out += (char)buffer[index];
    }
}

ChannelID TParser::constructID(const ChannelType type, const uint8_t channel)
{
    return {type, channel};
}

AxisExtentionType TParser::getExtentionTypeFromStr(char *buffer, const size_t length, size_t &index)
{
    char type_char = toupper(TString::getCharAt(buffer, length, index));
    switch (type_char)
    {
    case 'I':
        return AxisExtentionType::Time;
    case 'S':
        return AxisExtentionType::Speed;
    }

    index++;
    return AxisExtentionType::None;
}

bool TParser::getRampTypeFromStr(char *buffer, const size_t length, size_t &index, AxisRampType &rampType)
{
    char first = toupper(TString::getCharAt(buffer, length, index));
    switch (first) // Decode what Ramp type it is
    {
    case '<':
    {
        rampType = AxisRampType::In;
    }
    break;
    case '>':
    {
        rampType = AxisRampType::Out;
    }
    break;
    case '=':
    {
        rampType = AxisRampType::InOut;
    }
    break;
    default:
        rampType = AxisRampType::None;
        return false;
    }

    index++;
    return true;
}

ChannelID TParser::getIDFromStr(char *buffer, const size_t length, size_t &startIndex)
{
    char type_char = toupper(TString::getCharAt(buffer, length, startIndex++));
    uint8_t channel = static_cast<uint8_t>(toupper(TString::getCharAt(buffer, length, startIndex++)) - '0'); // get channel number 0 - 9
    ChannelType type = ChannelType::None;
    switch (type_char) // get channel type
    {
    case 'L':
        type = ChannelType::Linear;
        break;
    case 'R':
        type = ChannelType::Rotation;
        break;
    case 'V':
        type = ChannelType::Vibration;
        break;
    case 'A':
        type = ChannelType::Auxiliary;
        break;
    default:
        type = ChannelType::None;
    }
    return {type, channel};
}

CommandType TParser::getCommandType(char *buffer, const size_t length, size_t startIndex)
{
    // Switch between command types
    switch (toupper(TString::getCharAt(buffer, length, startIndex)))
    {
    // Axis commands
    case 'L':
    case 'R':
    case 'V':
    case 'A':
        return CommandType::Axis;
    // Device commands
    case 'D':
        return CommandType::Device;
    // Setup commands
    case '$':
        return CommandType::Setup;
    }
    return CommandType::None;
}

size_t TParser::getNextCommand(deque<char> inputBuffer, char *buffer, size_t buffer_length)
{
    size_t index = 0;
    bool blevel = false;
    while (!inputBuffer.empty() && (index < buffer_length - 1))
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
    
    return index;
}

bool TParser::parseAxisCommand(char *buffer, const size_t length, AxisCommand &out)
{
    size_t index = 0;
    ChannelID id = getIDFromStr(buffer, length, index);

    if (!idValid(id)) // make sure that the ID is valid if it isnt then the command is not valid
        return false;

    AxisExtentionType extentionType = AxisExtentionType::None;
    AxisRampType rampType = AxisRampType::None;
    AxisRampData rampIn = {};
    AxisRampData rampOut = {};
    float commandValue = 0;
    unsigned long commandExtention = 0;

    if (id.type == ChannelType::None)
        return false;

    size_t log_value;
    if (!TString::getNextTCodeFloat(commandValue, log_value, buffer, length, index))
        return false;

    while (true)
    {
        if (TString::isextention(TString::getCharAt(buffer, length, index)))
        {
            if (extentionType != AxisExtentionType::None)
                return false;

            if (!parseAxisExtention(buffer, length, index, extentionType, commandExtention))
                return false;
        }
        else if (TString::isramp(TString::getCharAt(buffer, length, index)))
        {            
            if (!parseAxisRamp(buffer, length, index, rampType, rampIn, rampOut))
                return false;
        }
        else
        {
            break;
        }
    }

    // if the command has been processed and there are still characters left over the command has not been processed correctly/the command is incorrect
    if (toupper(TString::getCharAt(buffer, length, index)) != '\0')
        return false;

    AxisData data;
    data.commandValue = commandValue;
    data.commandExtention = commandExtention;
    data.extentionType = extentionType;
    data.rampType = rampType;
    data.rampIn = rampIn;
    data.rampOut = rampOut;

    out.data = data;
    out.id = id;
    return true;
}

bool TParser::parseAxisExtention(char *buffer, const size_t length, size_t &index, AxisExtentionType &extentionType, unsigned long &commandExtention)
{
    extentionType = getExtentionTypeFromStr(buffer, length, index);

    size_t log_value;
    if (!TString::getNextInt(commandExtention, log_value, buffer, length, index))
        return false;

    return true;
}

bool TParser::parseAxisRamp(char *buffer, const size_t length, size_t &index, AxisRampType &rampType, AxisRampData &rampIn, AxisRampData &rampOut)
{
    //(<|>|=)(<tangent>(.<weight>))
    if (rampType == AxisRampType::InOut)
        return false;

    AxisRampType currentRampType;
    if (!getRampTypeFromStr(buffer, length, index, currentRampType))
        return false;
    if (currentRampType == rampType)
        return false;
     
    AxisRampData data;
    if (!parseAxisRampData(buffer, length, index, currentRampType, data))
        return false;

    if (currentRampType == AxisRampType::In || currentRampType == AxisRampType::InOut)
        rampIn = data;
    if (currentRampType == AxisRampType::Out || currentRampType == AxisRampType::InOut)
        rampOut = data;

    if (rampType != AxisRampType::None)
        rampType = AxisRampType::InOut;

    return true;
}

bool TParser::parseAxisRampData(char *buffer, const size_t length, size_t &index, const AxisRampType &rampType, AxisRampData &data)
{
    data = {0, true, 1 / 3.0f, false, /* autoTangent */ rampType != AxisRampType::InOut};
    if (!isdigit(TString::getCharAt(buffer, length, index)))
        return true;

    size_t logValue;
    float tangent;
    if (!TString::getNextTCodeFloat(tangent, logValue, buffer, length, index))
        return false;
    
    data.tangent = map(tangent, 0, 1, -0.999f, 0.999f);
    if (TString::getCharAt(buffer, length, index) != '.')
        return true;
    
    index++;
    if (!isdigit(TString::getCharAt(buffer, length, index)))
        return false;
    
    float weight;
    if (!TString::getNextTCodeFloat(weight, logValue, buffer, length, index))
        return false; 

    data.weight = constrain(weight, 0, 0.999f);
    data.hasWeight = true;
    return true;
}

bool TParser::parseSetupCommand(char *buffer, const size_t length, SetupCommand &out)
{
    size_t index = 0;
    if (toupper(TString::getCharAt(buffer, length, index++)) != '$')
        return false;

    ChannelID id = getIDFromStr(buffer, length, index); // get the ID

    if (!idValid(id))
        return false;

    if (toupper(TString::getCharAt(buffer, length, index++)) != '-')
        return false;

    unsigned long minValueLong; 
    size_t minValuelog;
    if (!TString::getNextInt(minValueLong,minValuelog,buffer, length, index)) // Get the first minimum value
        return false;

    if (toupper(TString::getCharAt(buffer, length, index++)) != '-')
        return false;

    unsigned long maxValueLong; 
    size_t maxValuelog;
    if (!TString::getNextInt(maxValueLong,maxValuelog,buffer, length, index)) // Get the first minimum value
        return false;

    if ((toupper(TString::getCharAt(buffer, length, index)) != '\0'))
        return false;

    float minValue = (double)minValueLong / pow10f(minValuelog);
    float maxValue = (double)maxValueLong / pow10f(maxValuelog);

    if (minValue > maxValue) // if the minimum is larger than the maximum the command is not valid
        return false;

    out.id = id;
    out.saveEntryData.min = minValue;
    out.saveEntryData.max = maxValue;
    out.saveEntryData.minLog = minValuelog;
    out.saveEntryData.maxLog = maxValuelog;
    return true;
}

bool TParser::parseDeviceCommand(char *buffer, const size_t length, DeviceCommand &out)
{
    size_t index = 0;
    if (toupper(TString::getCharAt(buffer, length, index++)) != 'D')
        return false;

    switch (toupper(TString::getCharAt(buffer, length, index))) // looks at the first char and checks if it matches the values
    {
    case 'S':
        out.type = DeviceCommandType::StopDevice;
        return true;
    case '0':
        out.type = DeviceCommandType::GetSoftwareVersion;
        return true;
    case '1':
        out.type = DeviceCommandType::GetTCodeVersion;
        return true;
    case '2':
        out.type = DeviceCommandType::GetAssignedAxisValues;
        return true;
    }

    out.type = DeviceCommandType::None;
    return false;
}