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
     * @brief TCodeContext class contains important references to essential managers and settings related to TCode.
     *
     * The TCodeContext class provides access to key components such as axis management, module management,
     * settings, and output streaming. It holds references to these components and offers both getter and setter
     * methods to manage them.
     */
    class TCodeContext {
    public:
        /**
         * @brief Constructs a new TCodeContext object with default or custom parameters.
         *
         * @param firmware The firmware version (default is defined by DEFAULT_FIRMWARE_NAME).
         * @param tcodeVersion The TCode version (default is defined by DEFAULT_TCODE_VERSION).
         * @param Filepath The file path to the TCode file (default is defined by DEFAULT_FILE_NAME).
         */
        TCodeContext(const char *firmware = DEFAULT_FIRMWARE_NAME,
                     const char *tcodeVersion = DEFAULT_TCODE_VERSION,
                     const char *Filepath = DEFAULT_FILE_NAME)
            : filepath(Filepath), firmwareVersion(firmware), tcodeVersion(tcodeVersion) {}

        /**
         * @brief Retrieves the file path of the TCode file.
         *
         * @return const char* The file path to the TCode file.
         */
        const char *getFilepath() { return filepath; }

        /**
         * @brief Retrieves the firmware version.
         *
         * @return const char* The firmware version.
         */
        const char *getFirmware() { return firmwareVersion; }

        /**
         * @brief Retrieves the TCode version.
         *
         * @return const char* The TCode version.
         */
        const char *getTCodeVersion() { return tcodeVersion; }

        /**
         * @brief Sets the Axis Manager to handle axis-related tasks.
         *
         * @param axisManager A pointer to the TCodeAxisManager object to manage axes.
         */
        void setAxisManager(Axis::TCodeAxisManager *axisManager);

        /**
         * @brief Sets the Module Manager to handle module-related tasks.
         *
         * @param moduleManager A pointer to the TCodeModuleManager object to manage modules.
         */
        void setModuleManager(Module::TCodeModuleManager *moduleManager);

        /**
         * @brief Sets the Settings Manager to manage TCode settings.
         *
         * @param settings A pointer to the TCodeSettingsInterface object to manage settings.
         */
        void setSettingManager(Settings::TCodeSettingsInterface *settings);

        /**
         * @brief Sets the Output Stream Manager to handle output-related tasks.
         *
         * @param stream A pointer to the TCodeIOutputStream object to manage output streams.
         */
        void setOutputStream(Output::TCodeIOutputStream *stream);

        /**
         * @brief Gets the current Axis Manager.
         *
         * @param axisManager A reference to a pointer that will point to the current Axis Manager.
         * @return true if the Axis Manager is set, false otherwise.
         */
        bool getAxisManager(Axis::TCodeAxisManager *&axisManager);

        /**
         * @brief Gets the current Module Manager.
         *
         * @param moduleManager A reference to a pointer that will point to the current Module Manager.
         * @return true if the Module Manager is set, false otherwise.
         */
        bool getModuleManager(Module::TCodeModuleManager *&moduleManager);

        /**
         * @brief Gets the current Settings Manager.
         *
         * @param settings A reference to a pointer that will point to the current Settings Manager.
         * @return true if the Settings Manager is set, false otherwise.
         */
        bool getSettingManager(Settings::TCodeSettingsInterface *&settings);

        /**
         * @brief Gets the current Output Stream Manager.
         *
         * @param stream A reference to a pointer that will point to the current Output Stream Manager.
         * @return true if the Output Stream Manager is set, false otherwise.
         */
        bool getOutputStream(Output::TCodeIOutputStream *&stream);

    private:
        const char *filepath;        ///< The file path to the TCode file.
        const char *firmwareVersion; ///< The firmware version.
        const char *tcodeVersion;    ///< The TCode version.

        Settings::TCodeSettingsInterface *settingManager = nullptr; ///< Pointer to the Settings Manager.
        Axis::TCodeAxisManager *axisManager = nullptr;              ///< Pointer to the Axis Manager.
        Module::TCodeModuleManager *moduleManager = nullptr;        ///< Pointer to the Module Manager.
        Output::TCodeIOutputStream *outputStream = nullptr;         ///< Pointer to the Output Stream Manager.
    };

}
