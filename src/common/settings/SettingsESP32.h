// Setting-Management-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 12/11/2023
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <deque>

#include "ISettings.h"
#include "../../internal/settings/TaggedDataContainer.h"

#define DEFAULT_JSON_FILE_SIZE 2048
#define DEFAULT_SETTING_CACHE_SIZE 64

using namespace std;
using namespace TCode;

class SettingsESP32 : public ISettings
{
public:
    SettingsESP32() = delete;
    SettingsESP32(const char *filepath) : filepath(filepath) {}
    //=================================================
    //             Interface definitions
    //=================================================
    bool init();
    bool isMounted();
    bool hasSetting(const char *setting);
    void reset();
    bool getSetting(const char *setting, bool &settingValue) { return getSettingTemplated<bool>(setting, settingValue); }
    bool getSetting(const char *setting, int &settingValue) { return getSettingTemplated<int>(setting, settingValue); }
    bool getSetting(const char *setting, long &settingValue) { return getSettingTemplated<long>(setting, settingValue); }
    bool getSetting(const char *setting, unsigned int &settingValue) { return getSettingTemplated<unsigned int>(setting, settingValue); }
    bool getSetting(const char *setting, unsigned long &settingValue) { return getSettingTemplated<unsigned long>(setting, settingValue); }
    bool getSetting(const char *setting, float &settingValue) { return getSettingTemplated<float>(setting, settingValue); }
    bool getSetting(const char *setting, char *settingValue, size_t &length) { return false; }
    bool getSetting(const char *setting, String &settingValue) { return false; }

    bool setSetting(const char *setting, const bool settingValue) { return setSettingTemplated<bool>(setting, settingValue); }
    bool setSetting(const char *setting, const int settingValue) { return setSettingTemplated<int>(setting, settingValue); }
    bool setSetting(const char *setting, const long settingValue) { return setSettingTemplated<long>(setting, settingValue); }
    bool setSetting(const char *setting, const unsigned int settingValue) { return setSettingTemplated<unsigned int>(setting, settingValue); }
    bool setSetting(const char *setting, const unsigned long settingValue) { return setSettingTemplated<unsigned long>(setting, settingValue); }
    bool setSetting(const char *setting, const float settingValue) { return setSettingTemplated<float>(setting, settingValue); }
    bool setSetting(const char *setting, const char *settingValue, const size_t length) { return false; }
    bool setSetting(const char *setting, const String settingValue) { return false; }

    bool getSystemUsage(SettingsUsage &out);
    //=================================================

private:
    const char *filepath;
    bool fileSystemMounted = false;
    bool foundFile = false;

    deque<TaggedDataContainer> cache;

    bool keyInCache(const char* setting);
    bool getValueFromCache(const char* setting, DataContainer& value);
    bool setValueToCache(const char* setting, DataContainer value);
    bool getFile(String &out);
    bool writeFile(const String &fileData);
    unsigned long getFileSize();
protected:
    template <typename T>
    bool getSettingTemplated(const char *setting, T &settingValue);

    template <typename T>
    bool setSettingTemplated(const char *setting, const T &settingValue);
};
