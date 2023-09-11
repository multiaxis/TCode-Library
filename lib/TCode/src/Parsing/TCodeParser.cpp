// TCode-Parser-Class-H v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 26/06/2022
// Please copy, share, learn, innovate, give attribution.
#ifndef TCODE_PARSER_CPP
#define TCODE_PARSER_CPP
#include "TCodeParser.h"

bool TCodeParser::idValidRange(const TCode_Channel_Type type, const int channel_int, int max_channel)
{
    if (max_channel >= TCODE_MAX_CHANNEL_COUNT)
        max_channel = TCODE_MAX_CHANNEL_COUNT - 1;
    if (max_channel < 0)
        max_channel = 0;

    if ((channel_int > max_channel) || (channel_int < 0)) // check if channel number is not out of range
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

bool TCodeParser::idValidRange(const TCode_ChannelID id, int max_channel)
{
    return idValidRange(id.type, id.channel, max_channel);
}

bool TCodeParser::idValid(const TCode_Channel_Type type, const int channel_int)
{
    return idValidRange(type, channel_int, TCODE_MAX_CHANNEL_COUNT - 1);
}

bool TCodeParser::idValid(const TCode_ChannelID id)
{
    return idValidRange(id.type, id.channel, TCODE_MAX_CHANNEL_COUNT - 1);
}

int TCodeParser::getStrfromID(unsigned char *buffer, const size_t length, const TCode_ChannelID& id)
{
    size_t index = 0;
    switch (id.type) // gets the Channel type char and appends to the output string
    {
    case TCode_Channel_Type::Auxiliary:
        *(buffer + index) = 'A';
        break;
    case TCode_Channel_Type::Linear:
        *(buffer + index) = 'L';
        break;
    case TCode_Channel_Type::Rotation:
        *(buffer + index) = 'R';
        break;
    case TCode_Channel_Type::Vibration:
        *(buffer + index) = 'V';
        break;
    default:
        *(buffer + index) = '?';
    }
    index++;
    index += uintToStr(id.channel, buffer, length, index);

    return index;
}

void TCodeParser::getStrfromID(const TCode_ChannelID& id,String &out)
{
    unsigned char buffer[10];
    getStrfromID(buffer,10,id);
    size_t index = 0;
    for(; index < 10; index++)
    {
        out += buffer[index];
    }
}


TCode_ChannelID TCodeParser::constructID(const TCode_Channel_Type type, const uint8_t channel_int)
{
    return {type, channel_int};
}

TCode_Axis_Extention_Type TCodeParser::getExtentionTypeFromStr(unsigned char *buffer, const size_t length, size_t &startIndex)
{
    char type_char = toupper(getCharAt(buffer, length, startIndex));
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

TCode_Axis_Ramp_Type TCodeParser::getRampTypeFromStr(unsigned char *buffer, const size_t length, size_t &startIndex)
{
    TCode_Axis_Ramp_Type rampType = TCode_Axis_Ramp_Type::None;
    char first = toupper(getCharAt(buffer, length, startIndex));
    char second = toupper(getCharAt(buffer, length, startIndex + 1));
    switch (first) // Decode what Ramp type it is
    {
    case '<':
        if (second == '>')
        {
            startIndex += 2;
            rampType = TCode_Axis_Ramp_Type::EaseInOut;
        }
        else
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
    }
    return rampType;
}

TCode_ChannelID TCodeParser::getIDFromStr(unsigned char *buffer, const size_t length, size_t &startIndex)
{
    char type_char = toupper(getCharAt(buffer, length, startIndex++));
    uint8_t channel_int = static_cast<uint8_t>(toupper(getCharAt(buffer, length, startIndex++)) - '0'); // get channel number 0 - 9
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
    return {type, channel_int};
}

TCode_Command_Type TCodeParser::getCommandType(unsigned char *buffer, const size_t length, size_t startIndex)
{
    // Switch between command types
    switch (toupper(getCharAt(buffer, length, startIndex)))
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

bool TCodeParser::parseAxisCommand(unsigned char *buffer, const size_t length, TCode_Axis_Command &out)
{
    bool valid = true;
    size_t index = 0;
    TCode_ChannelID id = getIDFromStr(buffer, length, index);
    TCode_Axis_Extention_Type extentionType = TCode_Axis_Extention_Type::None;
    TCode_Axis_Ramp_Type rampType = TCode_Axis_Ramp_Type::Linear;
    long command_value = 0;
    long command_value_extention = 0;

    if (id.type == TCode_Channel_Type::None)
        valid = false;

    command_value = getNextInt(buffer, length, index);
    if (command_value == -1)
        valid = false;
    command_value = constrain(command_value, 0, TCODE_MAX_AXIS);

    if (isextention(getCharAt(buffer, length, index)))
    {
        extentionType = getExtentionTypeFromStr(buffer, length, index);
        command_value_extention = getNextInt(buffer, length, index);
        if (command_value_extention == -1)
            valid = false;
        command_value_extention = constrain(command_value_extention, 0, TCODE_MAX_AXIS_MAGNITUDE);
    }

    if (isramp(getCharAt(buffer, length, index)))
    {
        rampType = getRampTypeFromStr(buffer, length, index);
    }

    if (rampType == TCode_Axis_Ramp_Type::None)
        valid = false;
    if (extentionType == TCode_Axis_Extention_Type::None)
        valid = false;
    if (toupper(getCharAt(buffer, length, index)) != '\0') // if the command has been processed and there are still characters left over the command has not been processed correctly/the command is incorrect
        valid = false;
    if (!idValid(id)) // make sure that the ID is valid if it isnt then the command is not valid
        valid = false;

    TCode_Axis_Data data;
    data.command_value = command_value;
    data.command_value_extention = command_value_extention;
    data.extention_type = extentionType;
    data.ramp_type = rampType;

    out.Data = data;
    out.ID = id;

    return valid;
}

bool TCodeParser::parseSetupCommand(unsigned char *buffer, const size_t length, TCode_Setup_Command &out)
{
    bool valid = true;
    size_t index = 0;
    if (toupper(getCharAt(buffer, length, index++)) != '$')
        valid = false;

    TCode_ChannelID id = getIDFromStr(buffer, length, index); // get the ID
    if (toupper(getCharAt(buffer, length, index++)) != '-')
        valid = false;

    long min_value = getNextInt(buffer, length, index); // Get the first minimum value

    if (toupper(getCharAt(buffer, length, index++)) != '-')
        valid = false;

    long max_value = getNextInt(buffer, length, index); // Get the seccond maximum value
    if ((toupper(getCharAt(buffer, length, index)) != '\0') || (min_value == -1) || (max_value == -1) || (!idValid(id)))
        valid = false;
    min_value = constrain(min_value, 0, TCODE_MAX_AXIS); // constrain the values to the maximum axis constraint
    max_value = constrain(max_value, 0, TCODE_MAX_AXIS);
    if (min_value > max_value) // if the minimum is larger than the maximum the command is not valid
        valid = false;

    TCode_Save_Entry values = {static_cast<int>(min_value), static_cast<int>(max_value)};
    out = {id, values};
    return valid;
}

bool TCodeParser::parseExternalCommand(unsigned char *buffer, const size_t length, TCode_External_Command &out)
{

    size_t strLength = strlen((const char *)buffer);
    char *resizedBuffer = new char[strLength + 1];
    for (size_t i = 0; i < strLength; i++)
    {
        *(resizedBuffer + i) = *(buffer + i);
    }
    out.command = resizedBuffer;
    out.length = length;
    return true;
}

bool TCodeParser::parseDeviceCommand(unsigned char *buffer, const size_t length, TCode_Device_Command &out)
{
    size_t index = 0;
    if (toupper(getCharAt(buffer, length, index++)) != 'D')
        return false;

    switch (toupper(getCharAt(buffer, length, index))) // looks at the first char and checks if it matches the values
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

#endif