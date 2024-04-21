// TCode-Parser-Class-CPP v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 26/06/2022
// Please copy, share, learn, innovate, give attribution.
#include "TCodeParser.h"

bool Parser::idValidRange(const ChannelType type, const uint8_t channel, const uint8_t maxChannel)
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
    case ChannelType::AUXILIARY:
    case ChannelType::LINEAR:
    case ChannelType::ROTATION:
    case ChannelType::VIBRATION:
        break;
    default:
        return false;
    }

    return true;
}

bool Parser::idValidRange(const ChannelID id, const uint8_t maxChannel)
{
    return idValidRange(id.type, id.channel, maxChannel);
}

bool Parser::idValid(const ChannelType type, const uint8_t channel)
{
    return idValidRange(type, channel, TCODE_MAX_CHANNEL_COUNT - 1);
}

bool Parser::idValid(const ChannelID id)
{
    return idValidRange(id.type, id.channel, TCODE_MAX_CHANNEL_COUNT - 1);
}

int Parser::getStrfromID(char *buffer, const size_t length, const ChannelID &id)
{
    size_t index = 0;
    switch (id.type) // gets the Channel type char and appends to the output string
    {
    case ChannelType::AUXILIARY:
        buffer[index] = 'A';
        break;
    case ChannelType::LINEAR:
        buffer[index] = 'L';
        break;
    case ChannelType::ROTATION:
        buffer[index] = 'R';
        break;
    case ChannelType::VIBRATION:
        buffer[index] = 'V';
        break;
    default:
        buffer[index] = '?';
    }
    index++;
    index += TCodeCStringUtils::uintToStr(id.channel, buffer, length, index);

    return index;
}

void Parser::getStrfromID(const ChannelID &id, String &out)
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

ChannelID Parser::constructID(const ChannelType type, const uint8_t channel)
{
    return {type, channel};
}

AxisExtentionType Parser::getExtentionTypeFromStr(char *buffer, const size_t length, size_t &startIndex)
{
    char type_char = TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, startIndex));
    startIndex++;
    switch (type_char)
    {
    case 'I':
        return AxisExtentionType::TIME;
    case 'S':
        return AxisExtentionType::SPEED;
    }
    return AxisExtentionType::NONE;
}


void Parser::combineRampSegments(AxisRampType startingRampType, float startingRampValue, AxisRampType endingRampType, float endingRampValue, AxisRampType &outRampType, float &rampInValue, float &rampOutValue)
{
    if((startingRampType == AxisRampType::LINEAR) || (endingRampType == AxisRampType::LINEAR))
    {
        outRampType = AxisRampType::LINEAR;
        rampOutValue = -1;
        rampInValue = -1;
        return;
    }

    if(startingRampType == AxisRampType::NONE)
    {
        outRampType = endingRampType;
        rampOutValue = endingRampValue;
        rampInValue = -1;
        return;
    }

    if(endingRampType == AxisRampType::NONE)
    {
        outRampType = startingRampType;
        rampOutValue = -1;
        rampInValue = startingRampValue;
        return;
    }

    if(startingRampType != endingRampType)
    {
        if(startingRampType == AxisRampType::EASEIN)
        {
            rampOutValue = endingRampValue;
            rampInValue = startingRampValue;
            outRampType = AxisRampType::EASEINOUT;
        }
        else
        {
            rampOutValue = startingRampValue;
            rampInValue = endingRampValue;
            outRampType = AxisRampType::EASEINOUT;
        }
    }
}

bool Parser::parseRampSegment(char *buffer, const size_t length, size_t &startIndex, AxisRampType &rampType, float &rampValue)
{
    rampType = AxisRampType::NONE;
    char first = TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, startIndex));
    switch (first) // Decode what Ramp type it is
    {
    case '<':
    {
        startIndex++;
        rampType = AxisRampType::EASEIN;
    }
    break;
    case '>':
    {
        startIndex++;
        rampType = AxisRampType::EASEOUT;
    }
    break;
    case '=':
    {
        startIndex++;
        rampType = AxisRampType::LINEAR;
    }
    break;
    default:
        return false;
    }

    if(TCodeCStringUtils::isnumber(TCodeCStringUtils::getCharAt(buffer, length, startIndex+1)))
    {
        startIndex++;
        size_t log_value;
        if (!TCodeCStringUtils::getNextTCodeFloat(rampValue,log_value,buffer, length, startIndex))
            return false;
    }

    return true;
}

bool Parser::getRampTypeFromStr(char *buffer, const size_t length, size_t &startIndex, AxisRampType &rampType, float &rampInValue, float &rampOutValue)
{
    AxisRampType startingRampType = AxisRampType::NONE;
    AxisRampType endingRampType = AxisRampType::NONE;
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

ChannelID Parser::getIDFromStr(char *buffer, const size_t length, size_t &startIndex)
{
    char type_char = TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, startIndex++));
    uint8_t channel = static_cast<uint8_t>(TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, startIndex++)) - '0'); // get channel number 0 - 9
    ChannelType type = ChannelType::NONE;
    switch (type_char) // get channel type
    {
    case 'L':
        type = ChannelType::LINEAR;
        break;
    case 'R':
        type = ChannelType::ROTATION;
        break;
    case 'V':
        type = ChannelType::VIBRATION;
        break;
    case 'A':
        type = ChannelType::AUXILIARY;
        break;
    default:
        type = ChannelType::NONE;
    }
    return {type, channel};
}

CommandType Parser::getCommandType(char *buffer, const size_t length, size_t startIndex)
{
    // Switch between command types
    switch (TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, startIndex)))
    {
    // Axis commands
    case 'L':
    case 'R':
    case 'V':
    case 'A':
        return CommandType::AXIS;
    // Device commands
    case 'D':
        return CommandType::DEVICE;
    // Setup commands
    case '$':
        return CommandType::SETUP;
    case '#':
        return CommandType::EXTERNAL;
    }
    return CommandType::NONE;
}

size_t Parser::getNextCommand(TCodeBuffer<char> *inputBuffer, char *buffer, size_t buffer_length)
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

size_t Parser::getNextCommand(char *inputBuffer, const size_t length , const size_t startIndex, char *outbuffer, const size_t outBufferLength)
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

bool Parser::parseAxisCommand(char *buffer, const size_t length, AxisCommand &out)
{
    size_t index = 0;
    ChannelID id = getIDFromStr(buffer, length, index);

    if (!idValid(id)) // make sure that the ID is valid if it isnt then the command is not valid
        return false;

    AxisExtentionType extentionType = AxisExtentionType::NONE;
    AxisRampType rampType = AxisRampType::LINEAR;
    float rampInValue = 0;
    float rampOutValue = 0;
    float commandValue = 0;
    unsigned long commandExtention = 0;

    if (id.type == ChannelType::NONE)
        return false;

    size_t log_value;
    if (!TCodeCStringUtils::getNextTCodeFloat(commandValue,log_value,buffer, length, index))
        return false;

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

    AxisData data;
    data.commandValue = commandValue;
    data.commandExtention = commandExtention;
    data.extentionType = extentionType;
    data.rampType = rampType;

    out.axisData = data;
    out.id = id;
    return true;
}

bool Parser::parseSetupCommand(char *buffer, const size_t length, SetupCommand &out)
{
    size_t index = 0;
    if (TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, index++)) != '$')
        return false;

    ChannelID id = getIDFromStr(buffer, length, index); // get the ID

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
    if(!TCodeCStringUtils::getNextInt(maxValueLong,maxValuelog,buffer, length, index)) // Get the first minimum value
        return false;

    if ((TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, index)) != '\0'))
        return false;

    float minValue = (double)minValueLong / (pow(10,minValuelog)-1);
    float maxValue = (double)maxValueLong / (pow(10,maxValuelog)-1);

    if (minValue > maxValue) // if the minimum is larger than the maximum the command is not valid
        return false;

    out.id = id;
    out.saveEntryData.min = minValue;
    out.saveEntryData.max = maxValue;
    out.saveEntryData.min_log = minValuelog;
    out.saveEntryData.max_log = maxValuelog;
    return true;
}

bool Parser::parseExternalCommand(char *buffer, const size_t length, ExternalCommand &out)
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

bool Parser::parseDeviceCommand(char *buffer, const size_t length, DeviceCommand &out)
{
    size_t index = 0;
    if (TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, index++)) != 'D')
        return false;

    switch (TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, index))) // looks at the first char and checks if it matches the values
    {
    case 'S':
        out.type = DeviceCommandType::STOPDEVICE;
        return true;
    case '0':
        out.type = DeviceCommandType::GETSOFTWAREVERSION;
        return true;
    case '1':
        out.type = DeviceCommandType::GETTCODEVERSION;
        return true;
    case '2':
        out.type = DeviceCommandType::GETAXISVALUES;
        return true;
    }

    out.type = DeviceCommandType::NONE;
    return false;
}