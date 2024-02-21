// TCode-Class-cpp v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 10/09/2023
// Please copy, share, learn, innovate, give attribution.
#include "TCode.h"

TCode::TCode(const char *firmware, const char *tcode_version) : firmwareVersion(firmware),
                                                                tcodeVersion(tcode_version),
                                                                filepath(DEFAULT_FILE_NAME)
{
}

void TCode::inputByte(const byte input)
{
    inputChar((const char)input);
}

void TCode::inputChar(const char input)
{
    bool success = inputBuffer.push(input); // pushes the inputted char to the buffer
    if (!success && inputBuffer.full())   // if the buffer is full then execute the first command and then push the char to the buffer
    {
        executeNextBufferCommand();
        inputBuffer.push(input);
    }

    if (input == '\n') // if a newline is encountered run all commands till the buffer is empty
    {
        while (!inputBuffer.empty())
            executeNextBufferCommand();

        if (useOverwrite)
        {
            while (!axisCommandBuffer.empty())
            {
                TCode_Axis_Command result;
                if (axisCommandBuffer.pop(result))
                {
                    runAxisCommand(result);
                }
            }
            axisCommandBuffer.clear();
        }
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

bool TCode::registerAxis(ITCodeAxis *axis)
{
    if (axis == nullptr)
    {
        return false;
    }

    if (getAxisFromName(axis->getName()) != nullptr)
    {
        return false;
    }

    if (getAxisFromID(axis->getChannelID()) != nullptr)
    {
        return false;
    }

    return axisBuffer.push(axis);
}

void TCode::axisWrite(const TCode_ChannelID &id, const int magnitude, const TCode_Axis_Extention_Type extentionValue, const long extMagnitude, const TCode_Axis_Ramp_Type rampType)
{
    ITCodeAxis *axis = getAxisFromID(id);
    if (axis != nullptr)
    {
        axis->set(magnitude, extentionValue, extMagnitude, rampType);
    }
}

void TCode::axisWrite(const char *name, const int magnitude, const TCode_Axis_Extention_Type extentionValue, const long extMagnitude, const TCode_Axis_Ramp_Type rampType)
{
    ITCodeAxis *axis = getAxisFromName(name);
    if (axis != nullptr)
    {
        axis->set(magnitude, extentionValue, extMagnitude, rampType);
    }
}

int TCode::axisRead(const TCode_ChannelID &channel_id)
{
    ITCodeAxis *axis = getAxisFromID(channel_id);
    if (axis != nullptr)
    {
        return axis->getPosition();
    }
    return -1;
}

int TCode::axisRead(const char *name)
{
    ITCodeAxis *axis = getAxisFromName(name);
    if (axis != nullptr)
    {
        return axis->getPosition();
    }
    return -1;
}

unsigned long TCode::axisLastCommandTime(const TCode_ChannelID &channel_id)
{
    ITCodeAxis *axis = getAxisFromID(channel_id);
    if (axis != nullptr)
    {
        return axis->getLastCommandTime();
    }
    return -1;
}

unsigned long TCode::axisLastCommandTime(const char *name)
{
    ITCodeAxis *axis = getAxisFromName(name);
    if (axis != nullptr)
    {
        return axis->getLastCommandTime();
    }
    return -1;
}

void TCode::update()
{
    for (int i = 0; i < buttonBuffer.count(); i++)
    {
        TCodeButton *temp = nullptr;
        if (buttonBuffer.get(i, temp))
        {
            if (temp->update())
            {
                String out = "#";
                out += temp->name;
                out += ":";
                if (temp->getState())
                    out += "1";
                else
                    out += "0";
                println(out);
            }
        }
    }
}

bool TCode::registerButton(TCodeButton *button)
{
    if (button == nullptr)
        return false;

    return buttonBuffer.push(button);
}

void TCode::stop()
{
    for (size_t i = 0; i < axisBuffer.count(); i++)
    {
        ITCodeAxis *temp;
        if (!axisBuffer.get(i, temp))
            break;
        if (temp->getChannelID().type == TCode_Channel_Type::Vibration)
            temp->set(0);
        else
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
    char charValue;
    if (!outputBuffer.pop(charValue))
        return '\0';
    return charValue;
}

size_t TCode::getChar(char *buffer, const size_t length)
{
    size_t length_to_read = min(length, outputBuffer.count());
    for (size_t i = 0; i < length_to_read; i++)
    {
        char charValue;
        if(outputBuffer.pop(charValue)){
            *(buffer) = charValue;
            buffer++;
        }
    }
    return length_to_read;
}

size_t TCode::externalAvailable()
{
    return externalCommandBuffer.count();
}

char TCode::getExternalChar()
{
    if (externalCommandBuffer.count() == 0)
        return '\0';
    char charValue;
    if (!externalCommandBuffer.pop(charValue))
        return '\0';
    return charValue;
}

size_t TCode::getExternalChar(char *buffer, const size_t length)
{
    size_t length_to_read = min(length, externalCommandBuffer.count());
    for (size_t i = 0; i < length_to_read; i++)
    {
        char charValue;
        if(outputBuffer.pop(charValue)){
            *(buffer) = charValue;
            buffer++;
        }
    }
    return length_to_read;
}

void TCode::setSettingManager(ISettings *settings)
{
    if (settings == nullptr)
        return;

    settingManager = settings;
    settingManager->init();
}

ISettings *TCode::getSettingManager()
{
    return settingManager;
}

ITCodeAxis *TCode::getAxisFromName(const char *name)
{
    for (size_t i = 0; i < axisBuffer.count(); i++)
    {
        ITCodeAxis *temp;
        if (!axisBuffer.get(i, temp))
            break;
        if (strcmp(temp->getName(), name) == 0)
        {
            return temp;
        }
    }

    return nullptr;
}

ITCodeAxis *TCode::getAxisFromID(const TCode_ChannelID &_id)
{
    for (size_t i = 0; i < axisBuffer.count(); i++)
    {
        ITCodeAxis *temp;
        if (!axisBuffer.get(i, temp))
            break;
        TCode_ChannelID id = temp->getChannelID();
        if ((id.channel != _id.channel) || (id.type != _id.type))
            continue;
        return temp;
    }

    return nullptr;
}

void TCode::executeNextBufferCommand()
{
    unsigned char command[MAX_COMMAND_BUFFER_LENGTH_COUNT] = {'\0'};
    size_t length = TCodeParser::getNextCommand(inputBuffer,command, MAX_COMMAND_BUFFER_LENGTH_COUNT);
    readCommand(command, length + 1);
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
        {
            if (!useOverwrite)
            {
                runAxisCommand(result);
            }
            else
            {
                bool found = false;
                for (int i = 0; i < axisCommandBuffer.count(); i++)
                {
                    TCode_Axis_Command check;
                    if (axisCommandBuffer.get(i, check))
                    {
                        if ((check.ID.channel == result.ID.channel) && (check.ID.type == result.ID.type))
                        {
                            found = true;
                            axisCommandBuffer.set(i, result);
                            break;
                        }
                    }
                }

                if (!found)
                    axisCommandBuffer.push(result);
            }
        }
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
    for (int i = 0; i < command.length, command.command[i] != '\0'; i++)
    {
        externalCommandBuffer.push(command.command[i]);
    }
    externalCommandBuffer.push('\n');
}

void TCode::setSaveValues(TCode_ChannelID &id, unsigned int min, unsigned int max)
{
    if (settingManager == nullptr)
    {
        print(F("TCODE : Setting Manager Is Null"));
        return;
    }

    ITCodeAxis *temp = getAxisFromID(id);
    if (temp != nullptr)
    {
        String str_id = "";
        TCodeParser::getStrfromID(id, str_id);
        String name = "AXIS-MIN-";
        name += str_id;
        if (!settingManager->hasSetting(name.c_str()))
        {
            settingManager->setSetting(name.c_str(), 0);
        }
        else
        {
            settingManager->setSetting(name.c_str(), min);
        }

        name = "AXIS-MAX-";
        name += str_id;
        if (!settingManager->hasSetting(name.c_str()))
        {
            settingManager->setSetting(name.c_str(), TCODE_MAX_AXIS);
        }
        else
        {
            settingManager->setSetting(name.c_str(), max);
        }
    }
}

void TCode::printSavedAxisValues()
{
    if (settingManager == nullptr)
    {
        print(F("TCODE : Setting Manager Is Null"));
        return;
    }

    for (size_t i = 0; i < axisBuffer.count(); i++)
    {
        ITCodeAxis *temp = nullptr;
        if (axisBuffer.get(i, temp))
        {
            String str_id = "";
            TCodeParser::getStrfromID(temp->getChannelID(), str_id);
            String name = "AXIS-MIN-";
            name += str_id;
            unsigned int min = 0;
            if (!settingManager->getSetting(name.c_str(), min))
            {
                settingManager->setSetting(name.c_str(), 0);
            }

            name = "AXIS-MAX-";
            name += str_id;
            unsigned int max = TCODE_MAX_AXIS;
            if (!settingManager->getSetting(name.c_str(), max))
            {
                settingManager->setSetting(name.c_str(), TCODE_MAX_AXIS);
            }

            print(str_id);
            print(' ');
            print(String(min));
            print(' ');
            print(String(max));
            print(' ');
            println(temp->getName());
        }
    }
}

void TCode::print(const char value)
{
    outputBuffer.push(value);
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

void TCode::println(const char value)
{
    outputBuffer.push(value);
    outputBuffer.push('\n');
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
