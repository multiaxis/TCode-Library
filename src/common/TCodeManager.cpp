// TCode-Class-CPP v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 10/09/2023
// Please copy, share, learn, innovate, give attribution.
#include "TCodeManager.h"
#include "../internal/axis/TAxis.h"
#include "../internal/input/TButton.h"
#include "../internal/TParser.h"
#include "../internal/utils/TMath.h"
#include "../internal/utils/TString.h"

using namespace TCode;

const int MAX_INPUT_BUFFER_LENGTH_COUNT = 512;

TCodeManager::TCodeManager(const char *firmware, const char *tcode_version)
    : firmwareVersion(firmware), tcodeVersion(tcode_version), filepath(DEFAULT_FILE_NAME)
{
    const int MAX_AXIS_COMMAND_BUFFER_COUNT = 20;
    axisCommandBuffer.reserve(MAX_AXIS_COMMAND_BUFFER_COUNT);
}

bool TCodeManager::registerAxis(const char *name, const AxisId &id, float defaultValue)
{
    if (getAxisFromName(name) != nullptr || getAxisFromId(id) != nullptr)
        return false;

    registeredAxes.push_back(TAxis(name, id, defaultValue));
    return true;
}

void TCodeManager::registerButton(unsigned int pin, const char* name, void (*callback)())
{
    return registeredInputs.push_back(new TButton(pin, name, callback));
}

void TCodeManager::read(const byte input)
{
    read((const char)input);
}

void TCodeManager::read(const char input)
{
    static const int MAX_COMMAND_BUFFER_LENGTH_COUNT = 512; //TODO: private field
    static char commandBuffer[MAX_COMMAND_BUFFER_LENGTH_COUNT] = {'\0'};

    if (inputBuffer.size() == MAX_INPUT_BUFFER_LENGTH_COUNT) {
        size_t length = consumeNextCommandFromInputBuffer(commandBuffer, MAX_COMMAND_BUFFER_LENGTH_COUNT);
        runCommand(commandBuffer, length);
    }
    
    inputBuffer.push_back(input);
    if (input == '\n') {
        while (!inputBuffer.empty()) {
            size_t length = consumeNextCommandFromInputBuffer(commandBuffer, MAX_COMMAND_BUFFER_LENGTH_COUNT);
            runCommand(commandBuffer, length);
        }
    }
}

void TCodeManager::read(const String &input)
{
    for (int i = 0; i < input.length(); i++)
        read(input.charAt(i));
}

void TCodeManager::read(const char *input)
{
    size_t length = strlen(input);
    for (int i = 0; i < length; i++)
        read(input[i]);
}

void TCodeManager::write(const char value) const
{
    if(outputStream == nullptr)
        return;
    outputStream->print(value);
}

void TCodeManager::write(const char *value) const
{
    if(outputStream == nullptr)
        return;
    outputStream->print(value);
}

void TCodeManager::write(const __FlashStringHelper *value) const
{
    if(outputStream == nullptr)
        return;
    outputStream->print(value);
}

void TCodeManager::write(const String &value) const
{
    if(outputStream == nullptr)
        return;
    outputStream->print(value);
}

void TCodeManager::writeLine(const char value) const
{
    if(outputStream == nullptr)
        return;
    outputStream->println(value);
}

void TCodeManager::writeLine(const char *value) const
{
    if(outputStream == nullptr)
        return;
    outputStream->println(value);
}

void TCodeManager::writeLine(const __FlashStringHelper *value) const
{
    if(outputStream == nullptr)
        return;
    outputStream->println(value);
}

void TCodeManager::writeLine(const String &value) const
{
    if(outputStream == nullptr)
        return;
    outputStream->println(value);
}

void TCodeManager::clearBuffer()
{
    inputBuffer.clear();
}

void TCodeManager::setAxisData(const AxisId &id, const AxisData &data)
{
    TAxis *axis = getAxisFromId(id);
    if (axis != nullptr)
        axis->set(data);
}

float TCodeManager::getAxisPosition(const AxisId &id)
{
    TAxis *axis = getAxisFromId(id);
    if (axis != nullptr)
        return axis->getPosition();
        
    return NAN;
}

unsigned long TCodeManager::getAxisLastCommandTime(const AxisId &id)
{
    TAxis *axis = getAxisFromId(id);
    if (axis != nullptr)
        return axis->getLastCommandTime();

    return LONG_MAX;
}

void TCodeManager::update()
{
    for (size_t i = 0; i < registeredInputs.size(); i++)
        registeredInputs[i]->update(*this);
}

void TCodeManager::stop()
{
    for (size_t i = 0; i < registeredAxes.size(); i++)
        registeredAxes[i].stop();
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
        outputStream = stream;
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

TAxis *TCodeManager::getAxisFromId(const AxisId &id)
{
    for (size_t i = 0; i < registeredAxes.size(); i++)
    {
        TAxis *axis = &registeredAxes[i];
        if (axis->getId() == id)
            return axis;
    }

    return nullptr;
}

size_t TCodeManager::consumeNextCommandFromInputBuffer(char *buffer, const size_t length) {
    size_t index = 0;
    while (!inputBuffer.empty() && index < length - 1)
    {
        char c = inputBuffer.front();
        if (c == ' ' || c == '\n')
        {
            inputBuffer.pop_front();
            return true;
        }

        buffer[index++] = c;
        inputBuffer.pop_front();
    }
    
    buffer[index++] = '\0';
    return index;
}

void TCodeManager::runCommand(const char *buffer, const size_t length)
{
    CommandType type = TParser::getCommandType(buffer, length);

    switch (type)
    {
    case CommandType::Axis:
    {
        AxisCommand result;
        if (TParser::parseAxisCommand(buffer, length, result))
            runAxisCommand(result);
        break;
    }
    case CommandType::Device:
    {
        DeviceCommand result;
        if (TParser::parseDeviceCommand(buffer, length, result))
            runDeviceCommand(result);
        break;
    }
    case CommandType::Setup:
    {
        SetupCommand result;
        if (TParser::parseSetupCommand(buffer, length, result))
            runSetupCommand(result);
        break;
    }
    default:
        break;
    }
}

void TCodeManager::runAxisCommand(AxisCommand &command)
{
    setAxisData(command.id, command.data);
}

void TCodeManager::runDeviceCommand(DeviceCommand &command)
{
    switch (command.type)
    {
    case DeviceCommandType::StopDevice:
    {
        stop();
        writeLine("STOP");
    }
    break;
    case DeviceCommandType::GetTCodeVersion:
    {
        writeLine(tcodeVersion);
    }
    break;
    case DeviceCommandType::GetSoftwareVersion:
    {
        writeLine(firmwareVersion);
    }
    break;
    case DeviceCommandType::GetAssignedAxisValues:
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

void TCodeManager::setSaveValues(const AxisId &id, float minimum, float maximum, uint8_t minLog, uint8_t maxLog)
{
    if (settingManager == nullptr)
    {
        writeLine(F("TCODE : Setting Manager Is Null"));
        return;
    }
    
    if (getAxisFromId(id) == nullptr)
        return;

    minimum = constrain(minimum, 0.f, 1.f);
    maximum = constrain(maximum, 0.f, 1.f);

    const uint8_t TCODE_MAX_LOG = 15;
    minLog = min(minLog, TCODE_MAX_LOG);
    maxLog = min(maxLog, TCODE_MAX_LOG);
    
    String idString = TString::axisIdToString(id);
    String settingName = "AXIS-MIN-" + idString;
    settingManager->setSetting(settingName.c_str(), minimum);

    settingName = "AXIS-MAX-" + idString;
    settingManager->setSetting(settingName.c_str(), maximum);

    settingName = "AXIS-MIN-LOG-" + idString;
    settingManager->setSetting(settingName.c_str(), (int)minLog);

    settingName = "AXIS-MAX-LOG-" + idString;
    settingManager->setSetting(settingName.c_str(), (int)maxLog);
}

void TCodeManager::printSavedAxisValues()
{
    if (settingManager == nullptr)
    {
        writeLine(F("TCODE : Setting Manager Is Null"));
        return;
    }

    for (size_t i = 0; i < registeredAxes.size(); i++)
    {
        TAxis *axis = &registeredAxes[i];

        float min = 0.0f;
        int minLog = 4;
        float max = 1.0;
        int maxLog = 4;

        String idString = TString::axisIdToString(axis->getId());
        String settingName = "AXIS-MIN-" + idString;
        if (!settingManager->getSetting(settingName.c_str(), min))
            settingManager->setSetting(settingName.c_str(), 0.0f);
        
        settingName = "AXIS-MIN-LOG-" + idString;
        if (!settingManager->getSetting(settingName.c_str(), minLog))
            settingManager->setSetting(settingName.c_str(), 4);

        settingName = "AXIS-MAX-" + idString;        
        if (!settingManager->getSetting(settingName.c_str(), max))
            settingManager->setSetting(settingName.c_str(), 1.0f);

        settingName = "AXIS-MAX-LOG-" + idString;
        if (!settingManager->getSetting(settingName.c_str(), maxLog))
            settingManager->setSetting(settingName.c_str(), 4);

        uint8_t logOut;
        unsigned long tcodeMin = TMath::getTCodeFromFloat(min, minLog, logOut);
        unsigned long tcodeMax = TMath::getTCodeFromFloat(max, maxLog, logOut);

        write(idString);
        write(' ');
        write(String(tcodeMin));
        write(' ');
        write(String(tcodeMax));
        write(' ');
        writeLine(axis->getName());
    }
}