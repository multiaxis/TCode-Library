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


void TCodeParser::combineRampSegments(TCode_Axis_Ramp_Type startingRampType, int startingRampValue, TCode_Axis_Ramp_Type endingRampType, int endingRampValue, TCode_Axis_Ramp_Type &outRampType, int &rampInValue, int &rampOutValue)
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

bool TCodeParser::parseRampSegment(char *buffer, const size_t length, size_t &startIndex, TCode_Axis_Ramp_Type &rampType, int &rampValue)
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

    rampValue = -1;
    if(TCodeCStringUtils::isnumber(TCodeCStringUtils::getCharAt(buffer, length, startIndex+1)))
    {
        rampValue = TCodeCStringUtils::getNextInt(buffer,length,startIndex);
    }

    return true;
}

bool TCodeParser::getRampTypeFromStr(char *buffer, const size_t length, size_t &startIndex, TCode_Axis_Ramp_Type &rampType, int &rampInValue, int &rampOutValue)
{
    TCode_Axis_Ramp_Type startingRampType = TCode_Axis_Ramp_Type::None;
    TCode_Axis_Ramp_Type endingRampType = TCode_Axis_Ramp_Type::None;
    int startingRampValue;
    int endingRampValue;
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
    bool valid = true;
    size_t index = 0;
    TCode_ChannelID id = getIDFromStr(buffer, length, index);
    TCode_Axis_Extention_Type extentionType = TCode_Axis_Extention_Type::None;
    TCode_Axis_Ramp_Type rampType = TCode_Axis_Ramp_Type::Linear;
    int rampInValue = 0;
    int rampOutValue = 0;
    long command_value = 0;
    long commandExtention = 0;

    if (id.type == TCode_Channel_Type::None)
        valid = false;

    command_value = TCodeCStringUtils::getNextTCodeInt(buffer, length, index);
    if (command_value == -1)
        valid = false;
    command_value = constrain(command_value, 0, TCODE_MAX_AXIS);

    if (TCodeCStringUtils::isextention(TCodeCStringUtils::getCharAt(buffer, length, index)))
    {
        extentionType = getExtentionTypeFromStr(buffer, length, index);
        commandExtention = TCodeCStringUtils::getNextInt(buffer, length, index);
        if (commandExtention == -1)
            valid = false;
        commandExtention = constrain(commandExtention, 0, TCODE_MAX_AXIS_MAGNITUDE);
    }

    if(TCodeCStringUtils::isramp(TCodeCStringUtils::getCharAt(buffer, length, index)))
    {
        if(!getRampTypeFromStr(buffer, length, index, rampType, rampInValue, rampOutValue))
            valid = false;
    }

    if (TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, index)) != '\0') // if the command has been processed and there are still characters left over the command has not been processed correctly/the command is incorrect
        valid = false;
    if (!idValid(id)) // make sure that the ID is valid if it isnt then the command is not valid
        valid = false;

    TCode_Axis_Data data;
    data.commandValue = command_value;
    data.commandExtention = commandExtention;
    data.extentionType = extentionType;
    data.rampType = rampType;

    out.Data = data;
    out.ID = id;

    return valid;
}

bool TCodeParser::parseSetupCommand(char *buffer, const size_t length, TCode_Setup_Command &out)
{
    bool valid = true;
    size_t index = 0;
    if (TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, index++)) != '$')
        valid = false;

    TCode_ChannelID id = getIDFromStr(buffer, length, index); // get the ID
    if (TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, index++)) != '-')
        valid = false;

    long minValue = TCodeCStringUtils::getNextInt(buffer, length, index); // Get the first minimum value

    if (TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, index++)) != '-')
        valid = false;

    long maxValue = TCodeCStringUtils::getNextInt(buffer, length, index); // Get the seccond maximum value
    if ((TCodeCStringUtils::toupper(TCodeCStringUtils::getCharAt(buffer, length, index)) != '\0') || (minValue == -1) || (maxValue == -1) || (!idValid(id)))
        valid = false;
    minValue = constrain(minValue, 0, TCODE_MAX_AXIS); // constrain the values to the maximum axis constraint
    maxValue = constrain(maxValue, 0, TCODE_MAX_AXIS);
    if (minValue > maxValue) // if the minimum is larger than the maximum the command is not valid
        valid = false;

    TCode_Save_Entry values = {static_cast<int>(minValue), static_cast<int>(maxValue)};
    out = {id, values};
    return valid;
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
        out = {TCode_Device_Command_Type::StopDevice};
        return true;
    case '0':
        out = {TCode_Device_Command_Type::GetSoftwareVersion};
        return true;
    case '1':
        out = {TCode_Device_Command_Type::GetTCodeVersion};
        return true;
    case '2':
        out = {TCode_Device_Command_Type::GetAssignedAxisValues};
        return true;
    }

    out = {TCode_Device_Command_Type::None};
    return false;
}