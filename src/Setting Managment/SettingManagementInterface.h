// Setting-Management-Interface-Class-Hpp v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 12/11/2023
// Please copy, share, learn, innovate, give attribution.
#pragma once
#ifndef SETTING_MANAGEMENT_INTERFACE_HPP
#define SETTING_MANAGEMENT_INTERFACE_HPP
#include <Arduino.h>

/**
 * @brief Struct for getting size data on the file system
 * @param sizeOfFile Size of current settings file in bytes
 * @param spaceAvailable Space available in the SPIFFS file system
 * @param spaceUsed Space used in the SPIFFS file system
 */
struct SettingsUsage
{
    size_t sizeOfFile;
    size_t spaceAvailable;
    size_t spaceUsed;
};

/**
 * @brief Wrapper for handling settings
 * @paragraph provides an interface for saving settings
 */
class ISettings
{
public:
    /**
     * @brief Initializes the Settings class
     */
    virtual bool init() = 0;

    /**
     * @brief checks if the filesystem is mounted and filepath created
     * @return true if filesystem is mounted
     */
    virtual bool isMounted() = 0;

    /**
     * @brief checks if the given setting is in the settings file
     * @param setting C-String representation of the setting being accessed
     * @return true if setting is in the settings file
     */
    virtual bool hasSetting(const char *setting) = 0;

    /**
     * Overwrites stored settings
     */
    virtual void reset() = 0;

    /**
     * @brief gets the value for a setting
     * @param setting C-String representation of the setting name being accessed
     * @param settingValue setting being obtained
     * @return true if setting is found
     */
    virtual bool getSetting(const char *setting, bool &settingValue) = 0;
    virtual bool getSetting(const char *setting, int &settingValue) = 0;
    virtual bool getSetting(const char *setting, long &settingValue) = 0;
    virtual bool getSetting(const char *setting, unsigned int &settingValue) = 0;
    virtual bool getSetting(const char *setting, unsigned long &settingValue) = 0;
    virtual bool getSetting(const char *setting, float &settingValue) = 0;
    virtual bool getSetting(const char *setting, char *settingValue, size_t &length) = 0;
    virtual bool getSetting(const char *setting, String &settingValue) = 0;

    /**
     * @brief gets the value for a setting
     * @param setting C-String representation of the setting name being accessed
     * @param settingValue setting being stored
     * @return true if setting is set
     */
    virtual bool setSetting(const char *setting, const bool &settingValue) = 0;
    virtual bool setSetting(const char *setting, const int &settingValue) = 0;
    virtual bool setSetting(const char *setting, const long &settingValue) = 0;
    virtual bool setSetting(const char *setting, const unsigned int &settingValue) = 0;
    virtual bool setSetting(const char *setting, const unsigned long &settingValue) = 0;
    virtual bool setSetting(const char *setting, const float &settingValue) = 0;
    virtual bool setSetting(const char *setting, const char *settingValue, const size_t &length) = 0;
    virtual bool setSetting(const char *setting, const String &settingValue) = 0;

    /**
     * @brief gets the system usage of the Settings system
     * @param out the struct used to store the settings details
     * @return returns false if the filesystem is not mounted
     */
    virtual bool getSystemUsage(SettingsUsage &out);
};

#endif