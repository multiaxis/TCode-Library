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

TCode_Axis_Extention_Type TCodeParser::getExtentionTypeFromStr(char *buffer, const size_t length, size_t &startIndex)
{
    char type_char = TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, startIndex));
    startIndex++;
    switch (type_char)
    {
    case 'I':
        return TCode_Axis_Extention_Type::Time;
    case 'S':
        return TCode_Axis_Extention_Type::Speed;
    }
    return TCode_Axis_Extention_Type::None;
}


void TCodeParser::combineRampSegments(TCode_Axis_Ramp_Type startingRampType, float startingRampValue, TCode_Axis_Ramp_Type endingRampType, float endingRampValue, TCode_Axis_Ramp_Type &outRampType, float &rampInValue, float &rampOutValue)
{
    if((startingRampType == TCode_Axis_Ramp_Type::Linear) || (endingRampType == TCode_Axis_Ramp_Type::Linear))
    {
        outRampType = TCode_Axis_Ramp_Type::Linear;
        rampOutValue = -1;
        rampInValue = -1;
        return;
    }

    if(startingRampType == TCode_Axis_Ramp_Type::None)
    {
        outRampType = endingRampType;
        rampOutValue = endingRampValue;
        rampInValue = -1;
        return;
    }

    if(endingRampType == TCode_Axis_Ramp_Type::None)
    {
        outRampType = startingRampType;
        rampOutValue = -1;
        rampInValue = startingRampValue;
        return;
    }

    if(startingRampType != endingRampType)
    {
        if(startingRampType == TCode_Axis_Ramp_Type::EaseIn)
        {
            rampOutValue = endingRampValue;
            rampInValue = startingRampValue;
            outRampType = TCode_Axis_Ramp_Type::EaseInOut;
        }
        else
        {
            rampOutValue = startingRampValue;
            rampInValue = endingRampValue;
            outRampType = TCode_Axis_Ramp_Type::EaseInOut;
        }
    }
}

bool TCodeParser::parseRampSegment(char *buffer, const size_t length, size_t &startIndex, TCode_Axis_Ramp_Type &rampType, float &rampValue)
{
    rampType = TCode_Axis_Ramp_Type::None;
    char first = TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, startIndex));
    switch (first) // Decode what Ramp type it is
    {
    case '<':
    {
        startIndex++;
        rampType = TCode_Axis_Ramp_Type::EaseIn;
    }
    break;
    case '>':
    {
        startIndex++;
        rampType = TCode_Axis_Ramp_Type::EaseOut;
    }
    break;
    case '=':
    {
        startIndex++;
        rampType = TCode_Axis_Ramp_Type::Linear;
    }
    break;
    default:
        return false;
    }

    if(TCodeCStringUtils::isnumber(TCodeCStringUtils::getCharAt(buffer, length, startIndex+1)))
    {
        startIndex++;
        unsigned long rampValueLong;
        size_t log_value;
        if (!TCodeCStringUtils::getNextTCodeInt(rampValueLong,log_value,buffer, length, startIndex))
            return false;
        rampValue = (double)rampValueLong / (pow(10,log_value)-1);
    }

    return true;
}

bool TCodeParser::getRampTypeFromStr(char *buffer, const size_t length, size_t &startIndex, TCode_Axis_Ramp_Type &rampType, float &rampInValue, float &rampOutValue)
{
    TCode_Axis_Ramp_Type startingRampType = TCode_Axis_Ramp_Type::None;
    TCode_Axis_Ramp_Type endingRampType = TCode_Axis_Ramp_Type::None;
    float startingRampValue;
    float endingRampValue;
    if(!parseRampSegment(buffer, length, startIndex, startingRampType, startingRampValue))
    {
        return false;
    }

    if(TCodeCStringUtils::isramp(TCodeCStringUtils::getCharAt(buffer, length, startIndex)))
    {
        parseRampSegment(buffer,length,startIndex,endingRampType,endingRampValue);
    }

    combineRampSegments(startingRampType, startingRampValue, endingRampType, endingRampValue, rampType, rampInValue, rampOutValue);

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
    TCode_Axis_Ramp_Type rampType = TCode_Axis_Ramp_Type::Linear;
    float rampInValue = 0;
    float rampOutValue = 0;
    float commandValue = 0;
    unsigned long commandExtention = 0;

    if (id.type == TCode_Channel_Type::None)
        return false;

    unsigned long commandValueLong;
    size_t log_value;
    if (!TCodeCStringUtils::getNextTCodeInt(commandValueLong,log_value,buffer, length, index))
        return false;
    commandValue = (double)commandValueLong / (pow(10,log_value)-1);

    if (TCodeCStringUtils::isextention(TCodeCStringUtils::getCharAt(buffer, length, index)))
    {
        extentionType = getExtentionTypeFromStr(buffer, length, index);
        if (!TCodeCStringUtils::getNextInt(commandExtention,log_value,buffer, length, index))
            return false;
    }

    if(TCodeCStringUtils::isramp(TCodeCStringUtils::getCharAt(buffer, length, index)))
    {
        if(!getRampTypeFromStr(buffer, length, index, rampType, rampInValue, rampOutValue))
            return false;
    }

    if (TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, index)) != '\0') // if the command has been processed and there are still characters left over the command has not been processed correctly/the command is incorrect
        return false;

    TCode_Axis_Data data;
    data.commandValue = commandValue;
    data.commandExtention = commandExtention;
    data.extentionType = extentionType;
    data.rampType = rampType;

    out.Data = data;
    out.ID = id;
    return true;
}

bool TCodeParser::parseSetupCommand(char *buffer, const size_t length, TCode_Setup_Command &out)
{
    size_t index = 0;
    if (TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, index++)) != '$')
        return false;

    TCode_ChannelID id = getIDFromStr(buffer, length, index); // get the ID

    if(!idValid(id))
        return false;

    if (TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, index++)) != '-')
        return false;

    unsigned long minValueLong; 
    size_t minValuelog;
    if(!TCodeCStringUtils::getNextInt(minValueLong,minValuelog,buffer, length, index)) // Get the first minimum value
        return false;

    if (TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, index++)) != '-')
        return false;

    unsigned long maxValueLong; 
    size_t maxValuelog;
    if(!TCodeCStringUtils::getNextInt(minValueLong,minValuelog,buffer, length, index)) // Get the first minimum value
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