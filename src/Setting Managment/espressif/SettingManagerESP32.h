// Setting-Management-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 12/11/2023
// Please copy, share, learn, innovate, give attribution.
#pragma once
#ifndef SETTING_MANAGER_ESP32_H
#define SETTING_MANAGER_ESP32_H
#include "../SettingManagementInterface.h"
#include "../../Utils/TCodeBuffer.h"
#include "../../Utils/TCodeTaggedDataContainer.h"
#include <SPIFFS.h>
#include <ArduinoJson.h>
#define DEFAULT_JSON_FILE_SIZE 2048
#define DEFAULT_SETTING_CACHE_SIZE 15


class SettingManagerESP32 : public ISettings
{
public:
    SettingManagerESP32() = delete;
    SettingManagerESP32(const char *filepath) : filepath(filepath), max_filesize(DEFAULT_JSON_FILE_SIZE) {}
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
    const size_t max_filesize;
    bool fileSystemMounted = false;
    bool foundFile = false;

    TCodeBuffer<TCodeTaggedDataContainer> cache {DEFAULT_SETTING_CACHE_SIZE};

    bool keyInCache(const char* setting);
    bool getValueFromCache(const char* setting,TCodeDataContainer& value);
    bool setValueToCache(const char* setting,TCodeDataContainer value);
    bool getFile(String &out);
    bool writeFile(const String &fileData);
    unsigned long getFileSize();
protected:
    template <class T>
    bool getSettingTemplated(const char *setting, T &settingValue);

    template <class T>
    bool setSettingTemplated(const char *setting, const T &settingValue);
};


template <class T>
inline bool SettingManagerESP32::getSettingTemplated(const char *setting, T &settingValue)
{
#ifdef DEBUG
    Serial.print("SM: Checking Cache");
#endif
    if(keyInCache(setting))
    {
#ifdef DEBUG
        Serial.print("SM: value found in cache:");
#endif
        TCodeDataContainer result;
        if(getValueFromCache(setting,result))
            return result.getValue<T>(settingValue);
    }



#ifdef DEBUG
    Serial.print(F("SM: getting setting "));
    Serial.println(setting);
#endif
    String file_data;
    if (!getFile(file_data))
    {
#ifdef DEBUG
        Serial.println(F("SM: error getting file data "));
#endif
        return false;
    }
    StaticJsonDocument<DEFAULT_JSON_FILE_SIZE> doc;
    DeserializationError error = deserializeJson(doc, file_data);
    if (error)
    {
#ifdef DEBUG
        Serial.print(F("SM: deserializeJson() failed: "));
        Serial.println(error.f_str());
#endif
        return false;
    }

#ifdef DEBUG
    Serial.print(F("SM: keys "));
    JsonObject documentRoot = doc.as<JsonObject>();
    for (JsonPair keyValue : documentRoot)
    {
        Serial.print(F("\""));
        Serial.print(keyValue.key().c_str());
        Serial.print("\":\"");
        Serial.print(keyValue.value().as<String>());
        Serial.println(F("\""));
    }
#endif

    if (!doc.containsKey(setting))
    {
#ifdef DEBUG
        Serial.println(F("SM: key not found"));
#endif
        return false;
    }

    settingValue = doc[setting].as<T>();

    return true;
}

template <class T>
inline bool SettingManagerESP32::setSettingTemplated(const char *setting, const T &settingValue)
{
    setValueToCache(setting,settingValue);
    

#ifdef DEBUG
    Serial.print(F("SM: setting setting "));
    Serial.print(setting);
    Serial.print(F(" to "));
    Serial.println(settingValue);
#endif

    String fileData;
    if (!getFile(fileData))
    {
        return false;
    }

    StaticJsonDocument<DEFAULT_JSON_FILE_SIZE> doc;
    DeserializationError error = deserializeJson(doc, fileData);
    if (error)
    {
#ifdef DEBUG
        Serial.print(F("SM: deserializeJson() failed: "));
        Serial.println(error.f_str());
#endif
        return false;
    }

    doc[setting] = settingValue;
    String out;
    serializeJson(doc, out);
    writeFile(out);
    return true;
}

#endif
