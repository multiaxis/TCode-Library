// TCode-Context-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 10/09/2023
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include <Arduino.h>

#include "../axis/axisManagement/TCodeAxisManager.h"
#include "../settings/settingsClasses/ISettings.h"
#include "../interface/interfaceManagement/interfaceManager.h"

#define DEFAULT_TCODE_VERSION "TCode v0.4"
#define DEFAULT_FIRMWARE_NAME "TCode"
#define DEFAULT_FILE_NAME "/spiffs/TCode.dat"

namespace TCode
{
    class TCodeContext
    {
    public:
        TCodeContext(const char *firmware = DEFAULT_FIRMWARE_NAME, const char *tcodeVersion = DEFAULT_TCODE_VERSION, const char *Filepath = DEFAULT_FILE_NAME) 
                    : firmwareVersion(firmware), tcodeVersion(tcodeVersion), filepath(Filepath) {}
        const char *getFilepath() {return filepath;}
        const char *getFirmware() {return firmwareVersion;}
        const char *getTCodeVersion() {return tcodeVersion;}

        void setAxisManager(TCodeAxisManager* axisManager);
        void setInterfaceManager(TCodeInterfaceManager* interfaceManager);
        void setSettingManager(ISettings *settings);
        void setOutputStream(Print *stream);

        bool getAxisManager(TCodeAxisManager* &axisManager);
        bool getInterfaceManager(TCodeInterfaceManager* &interfaceManager);
        bool getSettingManager(ISettings* &settings);
        bool getOutputStream(Print* &stream);
        

    private:
        const char *filepath;
        const char *firmwareVersion;
        const char *tcodeVersion;
        ISettings *settingManager;
        TCodeAxisManager *axisManager;
        TCodeInterfaceManager* interfaceManager;
        Print *outputStream;
    };
}


