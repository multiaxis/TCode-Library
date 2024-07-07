// TCode-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 10/09/2023
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include <Arduino.h>
#include <vector>

#include "../common/datatypes/CommandDataTypes.h"
#include "../common/datatypes/CommonDataTypes.h"
#include "../common/datatypes/EnumTypes.h"
#include "../internal/axis/TAxis.h"
#include "../internal/input/TInterfaceBase.h"
#include "../common/axisManagement/TCodeAxisManager.h"
#include "../common/settings/ISettings.h"

#define DEFAULT_TCODE_VERSION "TCode v0.4"
#define DEFAULT_FIRMWARE_NAME "TCode"
#define DEFAULT_FILE_NAME "/spiffs/TCode.dat"

namespace TCode
{

    class TCodeManager
    {
    public:
        TCodeManager(const char *firmware = DEFAULT_FIRMWARE_NAME, const char *tcodeVersion = DEFAULT_TCODE_VERSION);

        void write(const char value) const;
        void write(const char *value) const;
        void write(const __FlashStringHelper *value) const;
        void write(const String &value) const;
        void writeLine(const char value) const;
        void writeLine(const char *value) const;
        void writeLine(const __FlashStringHelper *value) const;
        void writeLine(const String &value) const;

        void clearInputBuffer();

        bool registerAxis(const AxisId &id, TAxis* axis);
        void setAxisData(const AxisId &id, const AxisData &data);
        float getAxisPosition(const AxisId &axisId);               
        unsigned long getAxisLastCommandTime(const AxisId &axisId);
        void stop();

        void registerInterface(TInterfaceBase *interface);
        void updateInterfaces();

        void setAxisManager(TCodeAxisManager* axisManager);
        void setInterfaceManager(); // TODO: implement
        void setSettingManager(ISettings *settings);
        void setOutputStream(Print *stream);

    private:
        const char *filepath;
        const char *firmwareVersion;
        const char *tcodeVersion;
        ISettings *settingManager;
        TCodeAxisManager *axisManager;

        vector<AxisCommandEvent> axisCommandBuffer;

        Print *outputStream;

        void runAxisCommand(AxisCommandEvent &command);
        void runDeviceCommand(DeviceCommandEvent &command);
        void runSetupCommand(SetupCommandEvent &command);

        void setSaveValues(const AxisId &id, float minimum, float maximum, uint8_t minLog = 4, uint8_t maxLog = 4);

        void printSavedAxisValues();
    };

};