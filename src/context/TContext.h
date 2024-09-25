// TCode-Context-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 10/09/2023
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include <Arduino.h>

#include "../axis/TAxisManager.h"
#include "../module/TModuleManager.h"
#include "../outputstream/TOutputStreamInterface.h"
#include "../settings/ISettings.h"

#define DEFAULT_TCODE_VERSION "TCode v0.4"
#define DEFAULT_FIRMWARE_NAME "TCode"
#define DEFAULT_FILE_NAME "/spiffs/TCode.dat"

namespace TCode {
    /**
     * @brief TCodeContext class used to contain important references to needed classes
     */
    class TCodeContext {
    public:
        TCodeContext(const char *firmware = DEFAULT_FIRMWARE_NAME, const char *tcodeVersion = DEFAULT_TCODE_VERSION, const char *Filepath = DEFAULT_FILE_NAME)
            : filepath(Filepath), firmwareVersion(firmware), tcodeVersion(tcodeVersion) {}
        const char *getFilepath() { return filepath; }
        const char *getFirmware() { return firmwareVersion; }
        const char *getTCodeVersion() { return tcodeVersion; }

        void setAxisManager(Axis::TCodeAxisManager *axisManager);
        void setModuleManager(Module::TCodeModuleManager *moduleManager);
        void setSettingManager(Settings::TCodeSettingsInterface *settings);
        void setOutputStream(Output::TCodeIOutputStream *stream);

        bool getAxisManager(Axis::TCodeAxisManager *&axisManager);
        bool getModuleManager(Module::TCodeModuleManager *&moduleManager);
        bool getSettingManager(Settings::TCodeSettingsInterface *&settings);
        bool getOutputStream(Output::TCodeIOutputStream *&stream);

    private:
        const char *filepath;
        const char *firmwareVersion;
        const char *tcodeVersion;
        Settings::TCodeSettingsInterface *settingManager = nullptr;
        Axis::TCodeAxisManager *axisManager = nullptr;
        Module::TCodeModuleManager *moduleManager = nullptr;
        Output::TCodeIOutputStream *outputStream = nullptr;
    };
}
