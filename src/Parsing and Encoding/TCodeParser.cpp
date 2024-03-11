// TCode-Parser-Class-CPP v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 26/06/2022
// Please copy, share, learn, innovate, give attribution.
#include "TCodeParser.h"

bool TCodeParser::idValidRange(const TCode_Channel_Type type, const uint8_t channel, const uint8_t maxChannel)
{
    uint8_t tempMaxChannel = maxChannel;
    if (maxChannel >= TCODE_MAX_CHANNEL_COUNT)
        tempMaxChannel = TCODE_MAX_CHANNEL_COUNT - 1;
    if (maxChannel < 0)
        tempMaxChannel = 0;

    if ((channel > tempMaxChannel) || (channel < 0)) // check if channel number is not out of range
        return false;

    switch (type) // check if the type is correct
    {
    case TCode_Channel_Type::Auxiliary:
    case TCode_Channel_Type::Linear:
    case TCode_Channel_Type::Rotation:
    case TCode_Channel_Type::Vibration:
        break;
    default:
        return false;
    }

    return true;
}

bool TCodeParser::idValidRange(const TCode_ChannelID id, const uint8_t maxChannel)
{
    return idValidRange(id.type, id.channel, maxChannel);
}

bool TCodeParser::idValid(const TCode_Channel_Type type, const uint8_t channel)
{
    return idValidRange(type, channel, TCODE_MAX_CHANNEL_COUNT - 1);
}

bool TCodeParser::idValid(const TCode_ChannelID id)
{
    return idValidRange(id.type, id.channel, TCODE_MAX_CHANNEL_COUNT - 1);
}

int TCodeParser::getStrfromID(char *buffer, const size_t length, const TCode_ChannelID &id)
{
    size_t index = 0;
    switch (id.type) // gets the Channel type char and appends to the output string
    {
    case TCode_Channel_Type::Auxiliary:
        buffer[index] = 'A';
        break;
    case TCode_Channel_Type::Linear:
        buffer[index] = 'L';
        break;
    case TCode_Channel_Type::Rotation:
        buffer[index] = 'R';
        break;
    case TCode_Channel_Type::Vibration:
        buffer[index] = 'V';
        break;
    default:
        buffer[index] = '?';
    }
    index++;
    index += TCodeCStringUtils::uintToStr(id.channel, buffer, length, index);

    return index;
}

void TCodeParser::getStrfromID(const TCode_ChannelID &id, String &out)
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

TCode_ChannelID TCodeParser::constructID(const TCode_Channel_Type type, const uint8_t channel)
{
    return {type, channel};
}

TCode_Axis_Extention_Type TCodeParser::getExtentionTypeFromStr(char *buffer, const size_t length, size_t &index)
{
    char type_char = TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, index));
    switch (type_char)
    {
    case 'I':
        return TCode_Axis_Extention_Type::Time;
    case 'S':
        return TCode_Axis_Extention_Type::Speed;
    }

    index++;
    return TCode_Axis_Extention_Type::None;
}

bool TCodeParser::getRampTypeFromStr(char *buffer, const size_t length, size_t &index, TCode_Axis_Ramp_Type &rampType)
{
    char first = TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, index));
    switch (first) // Decode what Ramp type it is
    {
    case '<':
    {
        rampType = TCode_Axis_Ramp_Type::In;
    }
    break;
    case '>':
    {
        rampType = TCode_Axis_Ramp_Type::Out;
    }
    break;
    case '=':
    {
        rampType = TCode_Axis_Ramp_Type::InOut;
    }
    break;
    default:
        rampType = TCode_Axis_Ramp_Type::None;
        return false;
    }

    index++;
    return true;
}

TCode_ChannelID TCodeParser::getIDFromStr(char *buffer, const size_t length, size_t &startIndex)
{
    char type_char = TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, startIndex++));
    uint8_t channel = static_cast<uint8_t>(TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, startIndex++)) - '0'); // get channel number 0 - 9
    TCode_Channel_Type type = TCode_Channel_Type::None;
    switch (type_char) // get channel type
    {
    case 'L':
        type = TCode_Channel_Type::Linear;
        break;
    case 'R':
        type = TCode_Channel_Type::Rotation;
        break;
    case 'V':
        type = TCode_Channel_Type::Vibration;
        break;
    case 'A':
        type = TCode_Channel_Type::Auxiliary;
        break;
    default:
        type = TCode_Channel_Type::None;
    }
    return {type, channel};
}

TCode_Command_Type TCodeParser::getCommandType(char *buffer, const size_t length, size_t startIndex)
{
    // Switch between command types
    switch (TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, startIndex)))
    {
    // Axis commands
    case 'L':
    case 'R':
    case 'V':
    case 'A':
        return TCode_Command_Type::Axis;
    // Device commands
    case 'D':
        return TCode_Command_Type::Device;
    // Setup commands
    case '$':
        return TCode_Command_Type::Setup;
    case '#':
        return TCode_Command_Type::External;
    }
    return TCode_Command_Type::None;
}

size_t TCodeParser::getNextCommand(TCodeBuffer<char> *inputBuffer, char *buffer, size_t buffer_length)
{
    size_t index = 0;
    bool blevel = false;
    while (!inputBuffer->empty() && (index < buffer_length - 1))
    {
        char charValue;
        if(!inputBuffer->peek(charValue))
            break;

        if(charValue == '\"')
            blevel = !blevel;

        if ((charValue == ' ') && !blevel)
        {
            inputBuffer->pop();
            break;
        }

        if (charValue == '\n')
        {
            inputBuffer->pop();
            break;
        }
        buffer[index++] = inputBuffer->pop();
    }
    return index;
}

size_t TCodeParser::getNextCommand(char *inputBuffer, const size_t length , const size_t startIndex, char *outbuffer, const size_t outBufferLength)
{
    size_t index = 0;
    size_t inputIndex = startIndex;
    bool blevel = false;
    while ((inputIndex < length - 1) && (index < outBufferLength - 1))
    {
        char charValue = inputBuffer[inputIndex];
        if(charValue == '\"')
            blevel = !blevel;

        if ((charValue == ' ') && !blevel)
        {
            inputIndex++;
            break;
        }

        if (charValue == '\n')
        {
            inputIndex++;
            break;
        }
        outbuffer[index++] = charValue;
        inputIndex++;
    }
    return index;
}

bool TCodeParser::parseAxisCommand(char *buffer, const size_t length, TCode_Axis_Command &out)
{
    size_t index = 0;
    TCode_ChannelID id = getIDFromStr(buffer, length, index);

    if (!idValid(id)) // make sure that the ID is valid if it isnt then the command is not valid
        return false;

    TCode_Axis_Extention_Type extentionType = TCode_Axis_Extention_Type::None;
    TCode_Axis_Ramp_Type rampType = TCode_Axis_Ramp_Type::None;
    TCode_Axis_Ramp_Data rampIn = {};
    TCode_Axis_Ramp_Data rampOut = {};
    float commandValue = 0;
    unsigned long commandExtention = 0;

    if (id.type == TCode_Channel_Type::None)
        return false;

    size_t log_value;
    if (!TCodeCStringUtils::getNextTCodeFloat(commandValue, log_value, buffer, length, index))
        return false;

    while (true)
    {
        if (TCodeCStringUtils::isextention(TCodeCStringUtils::getCharAt(buffer, length, index)))
        {
            if (extentionType != TCode_Axis_Extention_Type::None)
                return false;

            if (!parseAxisExtention(buffer, length, index, extentionType, commandExtention))
                return false;
        }
        else if (TCodeCStringUtils::isramp(TCodeCStringUtils::getCharAt(buffer, length, index)))
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
    if (TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, index)) != '\0')
        return false;

    TCode_Axis_Data data;
    data.commandValue = commandValue;
    data.commandExtention = commandExtention;
    data.extentionType = extentionType;
    data.rampType = rampType;
    data.rampIn = rampIn;
    data.rampOut = rampOut;

    out.Data = data;
    out.ID = id;
    return true;
}

bool TCodeParse::parseAxisExtention(char *buffer, const size_t length, size_t &index, TCode_Axis_Extention_Type &extentionType, long &commandExtention)
{
    extentionType = getExtentionTypeFromStr(buffer, length, index);

    size_t log_value;
    if (!TCodeCStringUtils::getNextInt(commandExtention, log_value, buffer, length, index))
        return false;
}

bool TCodeParser::parseAxisRamp(char *buffer, const size_t length, size_t &index, TCode_Axis_Ramp_Type &rampType, TCode_Axis_Ramp_Data &rampIn, TCode_Axis_Ramp_Data &rampOut)
{
    //(<|>|=)(<tangent>(.<weight>))
    if (rampType == TCode_Axis_Ramp_Type::InOut)
        return false;

    TCode_Axis_Ramp_Type currentRampType;
    if (!getRampTypeFromStr(buffer, length, index, currentRampType))
        return false;
    if (currentRampType == rampType)
        return false;
     
    TCode_Axis_Ramp_Data data;
    if (!parseAxisRampData(buffer, length, index, data))
        return false;

    if (currentRampType == TCode_Axis_Ramp_Type::In || currentRampType == TCode_Axis_Ramp_Type::InOut)
        rampIn = data;
    if (currentRampType == TCode_Axis_Ramp_Type::Out || currentRampType == TCode_Axis_Ramp_Type::InOut)
        rampOut = data;

    if (rampType != TCode_Axis_Ramp_Type::None)
        rampType = TCode_Axis_Ramp_Type::InOut;

    return true;
}

bool TCodeParser::parseAxisRampData(char *buffer, const size_t length, size_t &index, TCode_Axis_Ramp_Data &data)
{
    const float almostOne = 1 - TCodeFloatingOperations::Eps;

    data = {0, false, 1/3f, false}; 
    if (!TCodeCStringUtils::isnumber(TCodeCStringUtils::getCharAt(buffer, length, index)))
        return true;

    size_t log_value;
    if (!TCodeCStringUtils::getNextTCodeFloat(data.tangent, log_value, buffer, length, index))
        return false;
    
    data.tangent = clamp(data.tangent, -almostOne, almostOne);
    data.hasTangent = true;
    if (TCodeCStringUtils::getCharAt(buffer, length, index) != '.')
        return true;
    
    index++;
    if (!TCodeCStringUtils::isnumber(TCodeCStringUtils::getCharAt(buffer, length, index)))
        return false;
    
    if (!TCodeCStringUtils::getNextTCodeFloat(data.weight, log_value, buffer, length, index))
        return false; 

    data.weight = clamp(data.weight, 0, almostOne);
    data.hasWeight = true;
}

bool TCodeParser::parseSetupCommand(char *buffer, const size_t length, TCode_Setup_Command &out)
{
    size_t index = 0;
    if (TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, index++)) != '$')
        return false;

    TCode_ChannelID id = getIDFromStr(buffer, length, index); // get the ID

    if (!idValid(id))
        return false;

    if (TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, index++)) != '-')
        return false;

    unsigned long minValueLong; 
    size_t minValuelog;
    if (!TCodeCStringUtils::getNextInt(minValueLong,minValuelog,buffer, length, index)) // Get the first minimum value
        return false;

    if (TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, index++)) != '-')
        return false;

    unsigned long maxValueLong; 
    size_t maxValuelog;
    if (!TCodeCStringUtils::getNextInt(maxValueLong,maxValuelog,buffer, length, index)) // Get the first minimum value
        return false;

    if ((TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, index)) != '\0'))
        return false;

    float minValue = (double)minValueLong / (pow(10,minValuelog)-1);
    float maxValue = (double)maxValueLong / (pow(10,maxValuelog)-1);

    if (minValue > maxValue) // if the minimum is larger than the maximum the command is not valid
        return false;

    out.ID = id;
    out.Save.min = minValue;
    out.Save.max = maxValue;
    out.Save.min_log = minValuelog;
    out.Save.max_log = maxValuelog;
    return true;
}

bool TCodeParser::parseExternalCommand(char *buffer, const size_t length, TCode_External_Command &out)
{
    size_t strLength = strlen((const char *)buffer);
    char *resizedBuffer = new char[strLength + 1];
    for (size_t i = 0; i < strLength; i++)
    {
        *(resizedBuffer + i) = *(buffer + i);
    }
    out.command = resizedBuffer;
    out.length = strLength;
    return true;
}

bool TCodeParser::parseDeviceCommand(char *buffer, const size_t length, TCode_Device_Command &out)
{
    size_t index = 0;
    if (TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, index++)) != 'D')
        return false;

    switch (TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, index))) // looks at the first char and checks if it matches the values
    {
    case 'S':
        out.type = TCode_Device_Command_Type::StopDevice;
        return true;
    case '0':
        out.type = TCode_Device_Command_Type::GetSoftwareVersion;
        return true;
    case '1':
        out.type = TCode_Device_Command_Type::GetTCodeVersion;
        return true;
    case '2':
        out.type = TCode_Device_Command_Type::GetAssignedAxisValues;
        return true;
    }

    out.type = TCode_Device_Command_Type::None;
    return false;
}