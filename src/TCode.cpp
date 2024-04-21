// TCode-Class-CPP v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
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
                AxisCommand result;
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

void TCode::axisWrite(const ChannelID &id, const float magnitude, const AxisExtentionType extentionValue, const long extMagnitude, const AxisRampType rampType)
{
    ITCodeAxis *axis = getAxisFromID(id);
    if (axis != nullptr)
    {
        axis->set(magnitude, extentionValue, extMagnitude, rampType);
    }
}

void TCode::axisWrite(const char *name, const float magnitude, const AxisExtentionType extentionValue, const long extMagnitude, const AxisRampType rampType)
{
    ITCodeAxis *axis = getAxisFromName(name);
    if (axis != nullptr)
    {
        axis->set(magnitude, extentionValue, extMagnitude, rampType);
    }
}

float TCode::axisRead(const ChannelID &channel_id)
{
    ITCodeAxis *axis = getAxisFromID(channel_id);
    if (axis != nullptr)
    {
        return axis->getPosition();
    }
    return -1;
}

float TCode::axisRead(const char *name)
{
    ITCodeAxis *axis = getAxisFromName(name);
    if (axis != nullptr)
    {
        return axis->getPosition();
    }
    return -1;
}

unsigned long TCode::axisLastCommandTime(const ChannelID &channel_id)
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
    TCodeDataContainerSet outputvalues;
    for (int i = 0; i < externalInterfaces.count(); i++)
    {
        IExternalInterface *temp = nullptr;
        if (externalInterfaces.get(i, temp))
        {
            temp->update(*this,outputvalues);
        }
    }

    for (int i = 0; i < outputvalues.size(); i++)
    {
        TCodeTaggedDataContainer current = outputvalues[i];
        char buffer[64];
        if(TCodeEncoder::encodeValue(current,buffer,64))
        {
            println(buffer);
        }
    }
}

bool TCode::registerInterface(IExternalInterface *interface)
{
    if (interface == nullptr)
        return false;

    return externalInterfaces.push(interface);
}

void TCode::stop()
{
    for (size_t i = 0; i < axisBuffer.count(); i++)
    {
        ITCodeAxis *temp;
        if (!axisBuffer.get(i, temp))
            break;
        if (temp->getChannelID().type == ChannelType::VIBRATION)
            temp->set(0);
        else
            temp->stop();
    }
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
        if(externalCommandBuffer.pop(charValue)){
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

void TCode::setOutputStream(Print *stream)
{
    if(stream != nullptr)
    {
        outputStream = stream;
    }
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

ITCodeAxis *TCode::getAxisFromID(const ChannelID &_id)
{
    for (size_t i = 0; i < axisBuffer.count(); i++)
    {
        ITCodeAxis *temp;
        if (!axisBuffer.get(i, temp))
            break;
        ChannelID id = temp->getChannelID();
        if ((id.channel != _id.channel) || (id.type != _id.type))
            continue;
        return temp;
    }

    return nullptr;
}

void TCode::executeNextBufferCommand()
{
    char command[MAX_COMMAND_BUFFER_LENGTH_COUNT] = {'\0'};
    size_t length = Parser::getNextCommand(&inputBuffer,command, MAX_COMMAND_BUFFER_LENGTH_COUNT);
    readCommand(command, length + 1);
}

void TCode::readCommand(char *command, size_t length)
{
    CommandType type = Parser::getCommandType(command, length, 0); // find what command was read
    // Switch between command types
    switch (type) // depending on the command type found parse the inputted command and execute the correct command function
    {
    case CommandType::AXIS:
    {
        AxisCommand result;
        if (Parser::parseAxisCommand(command, length, result))
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
                    AxisCommand check;
                    if (axisCommandBuffer.get(i, check))
                    {
                        if ((check.id.channel == result.id.channel) && (check.id.type == result.id.type))
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
    case CommandType::DEVICE:
    {
        DeviceCommand result;
        if (Parser::parseDeviceCommand(command, length, result))
            runDeviceCommand(result);
        break;
    }
    case CommandType::SETUP:
    {
        SetupCommand result;
        if (Parser::parseSetupCommand(command, length, result))
            runSetupCommand(result);
        break;
    }
    case CommandType::EXTERNAL:
    {
        ExternalCommand result;
        if (Parser::parseExternalCommand(command, length, result))
            runExternalCommand(result);
        break;
    }
    default:
        break;
    }
}

void TCode::runAxisCommand(AxisCommand &command)
{
    axisWrite(command.id, command.axisData.commandValue, command.axisData.extentionType, command.axisData.commandExtention, command.axisData.rampType);
}

void TCode::runDeviceCommand(DeviceCommand &command)
{
    switch (command.type)
    {
    case DeviceCommandType::STOPDEVICE:
    {
        stop();
        println("STOP");
    }
    break;
    case DeviceCommandType::GETTCODEVERSION:
    {
        println(tcodeVersion);
    }
    break;
    case DeviceCommandType::GETSOFTWAREVERSION:
    {
        println(firmwareVersion);
    }
    break;
    case DeviceCommandType::GETAXISVALUES:
    {
        printSavedAxisValues();
    }
    break;
    }
}

void TCode::runSetupCommand(SetupCommand &command)
{
    setSaveValues(command.id, command.saveEntryData.min, command.saveEntryData.max);
    printSavedAxisValues();
}

void TCode::runExternalCommand(ExternalCommand &command)
{
    for (int i = 0; i < command.length, command.command[i] != '\0'; i++)
    {
        externalCommandBuffer.push(command.command[i]);
    }
    externalCommandBuffer.push('\n');
}

void TCode::setSaveValues(ChannelID &id, float min, float max, uint8_t min_log, uint8_t max_log)
{
    if (settingManager == nullptr)
    {
        print(F("TCODE : Setting Manager Is Null"));
        return;
    }

    if(min > 1.0)
        min = 1.0;
    if(min < 0.0)
        min = 0.0;

    if(max > 1.0)
        max = 1.0;
    if(max < 0.0)
        max = 0.0; 

    if(min_log > TCODE_MAX_LOG)
        min_log = TCODE_MAX_LOG;
    if(max_log > TCODE_MAX_LOG)
        max_log = TCODE_MAX_LOG;  
    

    ITCodeAxis *temp = getAxisFromID(id);
    if (temp != nullptr)
    {
        String str_id = "";
        Parser::getStrfromID(id, str_id);
        String name = "AXIS-MIN-";
        name += str_id;
        if (!settingManager->hasSetting(name.c_str()))
        {
            settingManager->setSetting(name.c_str(), 0.0f);
        }
        else
        {
            settingManager->setSetting(name.c_str(), min);
        }

        name = "AXIS-MAX-";
        name += str_id;
        if (!settingManager->hasSetting(name.c_str()))
        {
            settingManager->setSetting(name.c_str(), 1.0f);
        }
        else
        {
            settingManager->setSetting(name.c_str(), max);
        }

        name = "AXIS-MIN-LOG-";
        name += str_id;
        settingManager->setSetting(name.c_str(), (int)min_log);

        name = "AXIS-MAX-LOG-";
        name += str_id;
        settingManager->setSetting(name.c_str(), (int)max_log);
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
            float min = 0.0f;
            int min_log = 4;
            float max = 1.0;
            int max_log = 4;
            unsigned long tcode_min;
            unsigned long tcode_max;

            String str_id = "";
            Parser::getStrfromID(temp->getChannelID(), str_id);
            String name = "AXIS-MIN-";
            name += str_id;
            if (!settingManager->getSetting(name.c_str(), min))
            {
                settingManager->setSetting(name.c_str(), 0.0f);
            }
            

            name = "AXIS-MIN-LOG-";
            name += str_id;
            if (!settingManager->getSetting(name.c_str(), min_log))
            {
                settingManager->setSetting(name.c_str(), 4);
            }

            name = "AXIS-MAX-";
            name += str_id;
            
            if (!settingManager->getSetting(name.c_str(), max))
            {
                settingManager->setSetting(name.c_str(), 1.0f);
            }

            name = "AXIS-MAX-LOG-";
            name += str_id;
            if (!settingManager->getSetting(name.c_str(), max_log))
            {
                settingManager->setSetting(name.c_str(), 4);
            }
            unsigned char log_out;
            tcode_min = TCodeFloatingOperations::getTCodeFromFloat(min,min_log,log_out);
            tcode_max = TCodeFloatingOperations::getTCodeFromFloat(max,max_log,log_out);
            print(str_id);
            print(' ');
            print(String(tcode_min));
            print(' ');
            print(String(tcode_max));
            print(' ');
            println(temp->getName());
        }
    }
}

void TCode::print(const char value)
{
    if(outputStream == nullptr)
        return;
    outputStream->print(value);
}

void TCode::print(const char *value)
{
    if(outputStream == nullptr)
        return;
    outputStream->print(value);
}

void TCode::print(const __FlashStringHelper *value)
{
    if(outputStream == nullptr)
        return;
    outputStream->print(value);
}

void TCode::print(const String &value)
{
    if(outputStream == nullptr)
        return;
    outputStream->print(value);
}

void TCode::println(const char value)
{
    if(outputStream == nullptr)
        return;
    outputStream->println(value);
}

void TCode::println(const char *value)
{
    if(outputStream == nullptr)
        return;
    outputStream->println(value);
}

void TCode::println(const __FlashStringHelper *value)
{
    if(outputStream == nullptr)
        return;
    outputStream->println(value);
}

void TCode::println(const String &value)
{
    if(outputStream == nullptr)
        return;
    outputStream->println(value);
}
