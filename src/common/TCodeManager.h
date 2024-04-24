// TCode-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 10/09/2023
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include <Arduino.h>
#include <vector>
#include <deque>
#include "../internal/DataTypes.h"
#include "../internal/axis/TAxis.h"
#include "../internal/input/TInputBase.h"
#include "../common/settings/ISettings.h"

#define DEFAULT_TCODE_VERSION "TCode v0.4"
#define DEFAULT_FIRMWARE_NAME "TCode"
#define DEFAULT_FILE_NAME "/spiffs/TCode.dat"

namespace TCode {
    class TInputBase;
}

using namespace std;
using namespace TCode;

class TCodeManager
{
public:
    TCodeManager(const char *firmware = DEFAULT_FIRMWARE_NAME, const char *tcodeVersion = DEFAULT_TCODE_VERSION);

    bool registerAxis(const char *name, const AxisId &id, float defaultValue);
    void registerButton(unsigned int pin, const char* name, void (*callback)() = nullptr);

    void read(const byte input);
    void read(const char input);
    void read(const String &input);
    void read(const char *input);

    void write(const char value) const;
    void write(const char *value) const;
    void write(const __FlashStringHelper *value) const;
    void write(const String &value) const;
    void writeLine(const char value) const;
    void writeLine(const char *value) const;
    void writeLine(const __FlashStringHelper *value) const;
    void writeLine(const String &value) const;

    void clearBuffer();

    void setAxisData(const AxisId &id, const AxisData &data);
    float getAxisPosition(const AxisId &axisId);
    unsigned long getAxisLastCommandTime(const AxisId &axisId);

    void update();
    void stop();

    void setSettingManager(ISettings *settings);
    void setOutputStream(Print *stream);

private:
    const char *filepath;
    const char *firmwareVersion;
    const char *tcodeVersion;
    ISettings *settingManager;
    vector<TAxis> registeredAxes;
    vector<TInputBase*> registeredInputs;
    vector<AxisCommand> axisCommandBuffer;
    deque<char> inputBuffer;
    Print* outputStream;
    
    TAxis *getAxisFromName(const char *name);
    TAxis *getAxisFromId(const AxisId &id);

    size_t consumeNextCommandFromInputBuffer(char *buffer, const size_t length);

    void runCommand(const char *buffer, const size_t length);
    void runAxisCommand(AxisCommand &command);
    void runDeviceCommand(DeviceCommand &command);
    void runSetupCommand(SetupCommand &command);

    void setSaveValues(const AxisId &id, float minimum, float maximum, uint8_t minLog = 4, uint8_t maxLog = 4);

    void printSavedAxisValues();
};