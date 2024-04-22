// TCode-Class-CPP v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 10/09/2023
// Please copy, share, learn, innovate, give attribution.
#include "TCodeManager.h"
#include "../internal/axis/TAxis.h"
#include "../internal/input/TButton.h"
#include "../internal/TParser.h"
#include "../internal/utils/TMath.h"

using namespace TCode;

const int MAX_INPUT_BUFFER_LENGTH_COUNT = 512;

TCodeManager::TCodeManager(const char *firmware, const char *tcode_version) : firmwareVersion(firmware),
                                                                tcodeVersion(tcode_version),
                                                                filepath(DEFAULT_FILE_NAME)
{
    const int MAX_AXIS_COMMAND_BUFFER_COUNT = 20;

    axisCommandBuffer.reserve(MAX_AXIS_COMMAND_BUFFER_COUNT);
}

void TCodeManager::inputByte(const byte input)
{
    inputChar((const char)input);
}

void TCodeManager::inputChar(const char input)
{
    if (inputBuffer.size() == MAX_INPUT_BUFFER_LENGTH_COUNT)   // if the buffer is full then execute the first command and then push the char to the buffer
    {
        executeNextBufferCommand();
    }
    
    inputBuffer.push_back(input);
    if (input == '\n') // if a newline is encountered run all commands till the buffer is empty
    {
        while (!inputBuffer.empty())
            executeNextBufferCommand();

        if (useOverwrite)
        {
            while (!axisCommandBuffer.empty())
            {
                runAxisCommand(axisCommandBuffer.back());
                axisCommandBuffer.pop_back();
            }
            axisCommandBuffer.clear();
        }

        inputBuffer.clear();
    }
}

void TCodeManager::inputString(const String &input)
{
    for (int i = 0; i < input.length(); i++)
    {
        inputChar(input.charAt(i));
    }
}

void TCodeManager::inputCString(const char *&input)
{
    size_t length = strlen(input);
    for (int i = 0; i < length; i++)
    {
        inputChar(*(input + i));
    }
}

void TCodeManager::clearBuffer()
{
    inputBuffer.clear();
}

bool TCodeManager::registerAxis(const char *name, ChannelID channel, float defaultValue)
{
    if (getAxisFromName(name) != nullptr)
        return false;
    if (getAxisFromID(channel) != nullptr)
        return false;

    registeredAxes.push_back(TAxis(name, channel, defaultValue));
    return true;
}

void TCodeManager::axisWrite(const ChannelID &id, const AxisData &data)
{
    TAxis *axis = getAxisFromID(id);
    if (axis != nullptr)
        axis->set(data);
}

void TCodeManager::axisWrite(const char *name, const AxisData &data)
{
    TAxis *axis = getAxisFromName(name);
    if (axis != nullptr)
        axis->set(data);
}

float TCodeManager::axisRead(const ChannelID &channel_id)
{
    TAxis *axis = getAxisFromID(channel_id);
    if (axis != nullptr)
    {
        return axis->getPosition();
    }
    return -1;
}

float TCodeManager::axisRead(const char *name)
{
    TAxis *axis = getAxisFromName(name);
    if (axis != nullptr)
    {
        return axis->getPosition();
    }
    return -1;
}

unsigned long TCodeManager::axisLastCommandTime(const ChannelID &channel_id)
{
    TAxis *axis = getAxisFromID(channel_id);
    if (axis != nullptr)
    {
        return axis->getLastCommandTime();
    }
    return -1;
}

unsigned long TCodeManager::axisLastCommandTime(const char *name)
{
    TAxis *axis = getAxisFromName(name);
    if (axis != nullptr)
    {
        return axis->getLastCommandTime();
    }
    return -1;
}

void TCodeManager::update()
{
    for (size_t i = 0; i < registeredInputs.size(); i++)
    {
        registeredInputs[i]->update(*this);
    }
}

void TCodeManager::registerButton(unsigned int pin, const char* name, void (*callback)())
{
    return registeredInputs.push_back(new TButton(pin, name, callback));
}

void TCodeManager::stop()
{
    for (size_t i = 0; i < registeredAxes.size(); i++)
    {
        registeredAxes[i].stop();
    }
}

void TCodeManager::setSettingManager(ISettings *settings)
{
    if (settings == nullptr)
        return;

    settingManager = settings;
    settingManager->init();
}

void TCodeManager::setOutputStream(Print *stream)
{
    if(stream != nullptr)
    {
        outputStream = stream;
    }
}

TAxis *TCodeManager::getAxisFromName(const char *name)
{
    for (size_t i = 0; i < registeredAxes.size(); i++)
    {
        TAxis *axis = &registeredAxes[i];
        if (strcmp(axis->getName(), name) == 0)
            return axis;
    }

    return nullptr;
}

TAxis *TCodeManager::getAxisFromID(const ChannelID &id)
{
    for (size_t i = 0; i < registeredAxes.size(); i++)
    {
        TAxis *axis = &registeredAxes[i];
        if (axis->getChannelID().channel == id.channel & axis->getChannelID().type == id.type)
            return axis;
    }

    return nullptr;
}

void TCodeManager::executeNextBufferCommand()
{
    const int MAX_COMMAND_BUFFER_LENGTH_COUNT = 512;
    char command[MAX_COMMAND_BUFFER_LENGTH_COUNT] = {'\0'};
    size_t length = TParser::getNextCommand(inputBuffer, command, MAX_COMMAND_BUFFER_LENGTH_COUNT);
    readCommand(command, length + 1);
}

void TCodeManager::readCommand(char *command, size_t length)
{
    CommandType type = TParser::getCommandType(command, length, 0); // find what command was read
    // Switch between command types
    switch (type) // depending on the command type found parse the inputted command and execute the correct command function
    {
    case CommandType::AXIS:
    {
        AxisCommand result;
        if (TParser::parseAxisCommand(command, length, result))
        {
            if (!useOverwrite)
            {
                runAxisCommand(result);
            }
            else
            {
                bool found = false;
                for (int i = 0; i < axisCommandBuffer.size(); i++)
                {
                    AxisCommand check = axisCommandBuffer[i];
                    if ((check.id.channel == result.id.channel) && (check.id.type == result.id.type))
                    {
                        found = true;
                        axisCommandBuffer[i] = result;
                        break;
                    }
                }

                if (!found)
                    axisCommandBuffer.push_back(result);
            }
        }
        break;
    }
    case CommandType::DEVICE:
    {
        DeviceCommand result;
        if (TParser::parseDeviceCommand(command, length, result))
            runDeviceCommand(result);
        break;
    }
    case CommandType::SETUP:
    {
        SetupCommand result;
        if (TParser::parseSetupCommand(command, length, result))
            runSetupCommand(result);
        break;
    }
    default:
        break;
    }
}

void TCodeManager::runAxisCommand(AxisCommand &command)
{
    axisWrite(command.id, command.data);
}

void TCodeManager::runDeviceCommand(DeviceCommand &command)
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

void TCodeManager::runSetupCommand(SetupCommand &command)
{
    setSaveValues(command.id, command.saveEntryData.min, command.saveEntryData.max);
    printSavedAxisValues();
}

void TCodeManager::setSaveValues(ChannelID &id, float min, float max, uint8_t minLog, uint8_t maxLog)
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

    const int TCODE_MAX_LOG = 15;
    if(minLog > TCODE_MAX_LOG)
        minLog = TCODE_MAX_LOG;
    if(maxLog > TCODE_MAX_LOG)
        maxLog = TCODE_MAX_LOG;  
    
    if (getAxisFromID(id) == nullptr)
        return;
    
    String strId = "";
    TParser::getStrfromID(id, strId);
    String name = "AXIS-MIN-";
    name += strId;
    if (!settingManager->hasSetting(name.c_str()))
    {
        settingManager->setSetting(name.c_str(), 0.0f);
    }
    else
    {
        settingManager->setSetting(name.c_str(), min);
    }

    name = "AXIS-MAX-";
    name += strId;
    if (!settingManager->hasSetting(name.c_str()))
    {
        settingManager->setSetting(name.c_str(), 1.0f);
    }
    else
    {
        settingManager->setSetting(name.c_str(), max);
    }

    name = "AXIS-MIN-LOG-";
    name += strId;
    settingManager->setSetting(name.c_str(), (int)minLog);

    name = "AXIS-MAX-LOG-";
    name += strId;
    settingManager->setSetting(name.c_str(), (int)maxLog);
}

void TCodeManager::printSavedAxisValues()
{
    if (settingManager == nullptr)
    {
        print(F("TCODE : Setting Manager Is Null"));
        return;
    }

    for (size_t i = 0; i < registeredAxes.size(); i++)
    {
        TAxis *axis = &registeredAxes[i];

        float min = 0.0f;
        int minLog = 4;
        float max = 1.0;
        int maxLog = 4;
        unsigned long tcodeMin;
        unsigned long tcodeMax;

        String strId = "";
        TParser::getStrfromID(axis->getChannelID(), strId);
        String name = "AXIS-MIN-";
        name += strId;
        if (!settingManager->getSetting(name.c_str(), min))
        {
            settingManager->setSetting(name.c_str(), 0.0f);
        }
        

        name = "AXIS-MIN-LOG-";
        name += strId;
        if (!settingManager->getSetting(name.c_str(), minLog))
        {
            settingManager->setSetting(name.c_str(), 4);
        }

        name = "AXIS-MAX-";
        name += strId;
        
        if (!settingManager->getSetting(name.c_str(), max))
        {
            settingManager->setSetting(name.c_str(), 1.0f);
        }

        name = "AXIS-MAX-LOG-";
        name += strId;
        if (!settingManager->getSetting(name.c_str(), maxLog))
        {
            settingManager->setSetting(name.c_str(), 4);
        }
        unsigned char logOut;
        tcodeMin = TMath::getTCodeFromFloat(min,minLog,logOut);
        tcodeMax = TMath::getTCodeFromFloat(max,maxLog,logOut);
        print(strId);
        print(' ');
        print(String(tcodeMin));
        print(' ');
        print(String(tcodeMax));
        print(' ');
        println(axis->getName());
    }
}

void TCodeManager::print(const char value)
{
    if(outputStream == nullptr)
        return;
    outputStream->print(value);
}

void TCodeManager::print(const char *value)
{
    if(outputStream == nullptr)
        return;
    outputStream->print(value);
}

void TCodeManager::print(const __FlashStringHelper *value)
{
    if(outputStream == nullptr)
        return;
    outputStream->print(value);
}

void TCodeManager::print(const String &value)
{
    if(outputStream == nullptr)
        return;
    outputStream->print(value);
}

void TCodeManager::println(const char value)
{
    if(outputStream == nullptr)
        return;
    outputStream->println(value);
}

void TCodeManager::println(const char *value)
{
    if(outputStream == nullptr)
        return;
    outputStream->println(value);
}

void TCodeManager::println(const __FlashStringHelper *value)
{
    if(outputStream == nullptr)
        return;
    outputStream->println(value);
}

void TCodeManager::println(const String &value)
{
    if(outputStream == nullptr)
        return;
    outputStream->println(value);
}
