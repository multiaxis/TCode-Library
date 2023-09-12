// TCode-Class-cpp v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 10/09/2023
// Please copy, share, learn, innovate, give attribution.
#include "TCode.h"

TCode::TCode()
{
    firmwareVersion = DEFAULT_FIRMWARE_NAME;
    tcodeVersion = CURRENT_TCODE_VERSION;
    settingManager.init(DEFAULT_FILE_NAME);
}

TCode::TCode(const char *firmware)
{
    firmwareVersion = firmware;
    tcodeVersion = CURRENT_TCODE_VERSION;
    settingManager.init(DEFAULT_FILE_NAME);
}

TCode::TCode(const char *firmware, const char *tcode)
{
    firmwareVersion = firmware;
    tcodeVersion = tcode;
    settingManager.init(DEFAULT_FILE_NAME);
}

TCode::TCode(const char *firmware, const char *tcode_version, const char *_filepath)
{
    firmwareVersion = firmware;
    tcodeVersion = tcode_version;
    filepath = _filepath;
    settingManager.init(filepath);
}

void TCode::inputByte(const byte input)
{
    inputChar((const char)input);
}

void TCode::inputChar(const char input)
{
    bool success = inputBuffer.push(input); // pushes the inputted char to the buffer
    if (!success && inputBuffer.isFull())   // if the buffer is full then execute the first command and then push the char to the buffer
    {
        executeNextBufferCommand();
        inputBuffer.push(input);
    }

    if (input == '\n') // if a newline is encountered run all commands till the buffer is empty
    {
        while (!inputBuffer.isEmpty())
            executeNextBufferCommand();
        inputBuffer.clear();
    }
}

void TCode::inputString(const String &input)
{
    for (int i = 0; i < input.length(); i++)
    {
        inputChar(input.charAt(i));
    }
}

void TCode::inputCString(const char *&input)
{
    size_t length = strlen(input);
    for (int i = 0; i < length; i++)
    {
        inputChar(*(input + i));
    }
}

void TCode::clearBuffer()
{
    inputBuffer.clear();
}

bool TCode::registerAxis(TCodeAxis *axis)
{
    if (getAxisFromName(axis->getName()) != nullptr)
    {
        return false;
    }

    if (getAxisFromID(axis->getChannelID()) != nullptr)
    {
        return false;
    }

    bool success = axisBuffer.push(axis);
    return success;
}

void TCode::axisWrite(const TCode_ChannelID &id, const int magnitude, const TCode_Axis_Extention_Type extentionValue, const long extMagnitude, const TCode_Axis_Ramp_Type rampType)
{
    TCodeAxis *axis = getAxisFromID(id);
    if (axis != nullptr)
    {
        axis->set(magnitude, extentionValue, extMagnitude, rampType);
    }
}

void TCode::axisWrite(const char *name, const int magnitude, const TCode_Axis_Extention_Type extentionValue, const long extMagnitude, const TCode_Axis_Ramp_Type rampType)
{
    TCodeAxis *axis = getAxisFromName(name);
    if (axis != nullptr)
    {
        axis->set(magnitude, extentionValue, extMagnitude, rampType);
    }
}

int TCode::axisRead(const TCode_ChannelID &channel_id)
{
    TCodeAxis *axis = getAxisFromID(channel_id);
    if (axis != nullptr)
    {
        return axis->getPosition();
    }
    return -1;
}

int TCode::axisRead(const char *name)
{
    TCodeAxis *axis = getAxisFromName(name);
    if (axis != nullptr)
    {
        return axis->getPosition();
    }
    return -1;
}

void TCode::stop()
{
    for (size_t i = 0; i < axisBuffer.count(); i++)
    {
        TCodeAxis *temp;
        if (!axisBuffer.get(i, temp))
            break;
        temp->stop();
    }
}

size_t TCode::available()
{
    return outputBuffer.count();
}

char TCode::getChar()
{
    if (outputBuffer.count() == 0)
        return '\0';
    return outputBuffer.pop();
}

size_t TCode::getChar(char *buffer, const size_t length)
{
    size_t length_to_read = min(length, outputBuffer.count());
    for (size_t i = 0; i < length_to_read; i++)
    {
        *(buffer) = outputBuffer.pop();
        buffer++;
    }
    return length_to_read;
}

Settings *TCode::getSettingManager()
{
    return &settingManager;
}

TCodeAxis *TCode::getAxisFromName(const char *name)
{
    for (size_t i = 0; i < axisBuffer.count(); i++)
    {
        TCodeAxis *temp;
        if (!axisBuffer.get(i, temp))
            break;
        if (strcmp(temp->getName(), name) == 0)
            return temp;
        return temp;
    }

    return nullptr;
}

TCodeAxis *TCode::getAxisFromID(const TCode_ChannelID &id)
{
    for (size_t i = 0; i < axisBuffer.count(); i++)
    {
        TCodeAxis *temp;
        if (!axisBuffer.get(i, temp))
            break;
        TCode_ChannelID id = temp->getChannelID();
        if (id.channel != id.channel)
            continue;
        if (id.type != id.type)
            continue;
        return temp;
    }

    return nullptr;
}

void TCode::executeNextBufferCommand()
{
    unsigned char command[MAX_COMMAND_BUFFER_LENGTH_COUNT] = {'\0'};
    size_t index = 0;
    int blevel = 0;
    while (!inputBuffer.isEmpty() && (index < MAX_COMMAND_BUFFER_LENGTH_COUNT - 1))
    {
        if (inputBuffer.peek() == '[')
            blevel += 1;
        if (inputBuffer.peek() == ']')
            blevel -= 1;

        if (inputBuffer.peek() == ' ' && (blevel == 0))
        {
            inputBuffer.pop();
            break;
        }

        if (inputBuffer.peek() == '\n')
        {
            inputBuffer.pop();
            break;
        }

        if (blevel < 0)
            break;

        command[index++] = inputBuffer.pop();
    }

    readCommand(command, index + 1);
}

void TCode::readCommand(unsigned char *command, size_t length)
{
    TCode_Command_Type type = TCodeParser::getCommandType(command, length, 0); // find what command was read
    // Switch between command types
    switch (type) // depending on the command type found parse the inputted command and execute the correct command function
    {
    case TCode_Command_Type::Axis:
    {
        TCode_Axis_Command result;
        if (TCodeParser::parseAxisCommand(command, length, result))
            runAxisCommand(result);
        break;
    }
    case TCode_Command_Type::Device:
    {
        TCode_Device_Command result;
        if (TCodeParser::parseDeviceCommand(command, length, result))
            runDeviceCommand(result);
        break;
    }
    case TCode_Command_Type::Setup:
    {
        TCode_Setup_Command result;
        if (TCodeParser::parseSetupCommand(command, length, result))
            runSetupCommand(result);
        break;
    }
    case TCode_Command_Type::External:
    {
        TCode_External_Command result;
        if (TCodeParser::parseExternalCommand(command, length, result))
            runExternalCommand(result);
        break;
    }
    default:
        break;
    }
}

void TCode::runAxisCommand(TCode_Axis_Command &command)
{
    axisWrite(command.ID, command.Data.commandValue, command.Data.extentionType, command.Data.commandExtention, command.Data.rampType);
}

void TCode::runDeviceCommand(TCode_Device_Command &command)
{
    switch (command.type)
    {
    case TCode_Device_Command_Type::StopDevice:
    {
        stop();
        println("STOP");
    }
    break;
    case TCode_Device_Command_Type::GetTCodeVersion:
    {
        println(tcodeVersion);
    }
    break;
    case TCode_Device_Command_Type::GetSoftwareVersion:
    {
        println(firmwareVersion);
    }
    break;
    case TCode_Device_Command_Type::GetAssignedAxisValues:
    {
        printSavedAxisValues();
    }
    break;
    }
}

void TCode::runSetupCommand(TCode_Setup_Command &command)
{
    setSaveValues(command.ID, command.Save.min, command.Save.max);
    printSavedAxisValues();
}

void TCode::runExternalCommand(TCode_External_Command &command)
{
}

void TCode::setSaveValues(TCode_ChannelID &id, unsigned int min, unsigned int max)
{
    TCodeAxis *temp = getAxisFromID(id);
    if (temp != nullptr)
    {
        String str_id = "";
        TCodeParser::getStrfromID(id, str_id);
        String name = "AXIS-MIN-";
        name += str_id;
        if (!settingManager.hasSetting(name.c_str()))
        {
            settingManager.setSetting(name.c_str(), 0);
        }
        else
        {
            settingManager.setSetting(name.c_str(), min);
        }

        name = "AXIS-MAX-";
        name += str_id;
        if (!settingManager.hasSetting(name.c_str()))
        {
            settingManager.setSetting(name.c_str(), TCODE_MAX_AXIS);
        }
        else
        {
            settingManager.setSetting(name.c_str(), max);
        }
    }
}

void TCode::printSavedAxisValues()
{
    for (size_t i = 0; i < axisBuffer.count(); i++)
    {
        TCodeAxis *temp = nullptr;
        if (axisBuffer.get(i, temp))
        {
            String str_id;
            TCodeParser::getStrfromID(temp->getChannelID(), str_id);
            String name = "AXIS-MIN-";
            name += str_id;
            unsigned int min = 0;
            if (!settingManager.getSetting(name.c_str(), min))
            {
                settingManager.setSetting(name.c_str(), 0);
            }

            name = "AXIS-MAX-";
            name += str_id;
            unsigned int max = TCODE_MAX_AXIS;
            if (!settingManager.getSetting(name.c_str(), max))
            {
                settingManager.setSetting(name.c_str(), TCODE_MAX_AXIS);
            }

            name = "";
            name += str_id;
            name += '-';
            name += temp->getName();
            name += '-';
            name += String(min);
            name += '-';
            name += String(max);

            println(name);
        }
    }
}

void TCode::print(const char *value)
{
    size_t length = strlen(value);
    for (int i = 0; i < length; i++)
    {
        outputBuffer.push(*(value + i));
    }
}

void TCode::print(const __FlashStringHelper *value)
{
    char buffer[MAX_OUTPUT_BUFFER_LENGTH_COUNT];
    strncpy_P(buffer, (const char *)value, MAX_OUTPUT_BUFFER_LENGTH_COUNT - 1);
    print((const char *)buffer);
}

void TCode::print(const String &value)
{
    for (int i = 0; i < value.length(); i++)
    {
        outputBuffer.push(value.charAt(i));
    }
}

void TCode::println(const char *value)
{
    print(value);
    outputBuffer.push('\n');
}

void TCode::println(const __FlashStringHelper *value)
{
    print(value);
    outputBuffer.push('\n');
}

void TCode::println(const String &value)
{
    print(value);
    outputBuffer.push('\n');
}
