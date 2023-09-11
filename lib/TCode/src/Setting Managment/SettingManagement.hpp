// Setting-Management-Class-Hpp v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 9/09/2023
// Please copy, share, learn, innovate, give attribution.
#pragma once
#ifndef SETTING_MANAGEMENT_HPP
#define SETTING_MANAGEMENT_HPP
#include <SPIFFS.h>
#include <ArduinoJson.h>
#define JSON_FILE_SIZE 2048

class Settings
{
    const char *filepath;
    bool file_system_mounted = false;
    bool found_file = false;

public:
    bool init(const char *filepath);
    bool isMounted();
    bool hasSetting(const char *setting);

    template <class T>
    bool getSetting(const char *setting, T &setting_value);
    template <class T>
    bool setSetting(const char *setting, const T &setting_value);
    Settings() {}

private:
    bool getFile(String &out);
    bool writeFile(const String &file);
    virtual void createConfigFile(String &out);
    void outputSystemUsage();
    unsigned long getFileSize();
};

template <class T>
inline bool Settings::getSetting(const char *setting, T &setting_value)
{

#ifdef DEBUG
    Serial.print(F("SM: getting setting "));
    Serial.println(setting);
#endif

    StaticJsonDocument<JSON_FILE_SIZE> doc;
    String file_data;
    if (!getFile(file_data))
    {
#ifdef DEBUG
        Serial.println(F("SM: error getting file data "));
#endif
        return false;
    }

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
        for(JsonPair keyValue : documentRoot) {
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

    setting_value = doc[setting].as<T>();

    return true;
}

template <class T>
inline bool Settings::setSetting(const char *setting, const T &setting_value)
{

#ifdef DEBUG
    Serial.print(F("SM: setting setting "));
    Serial.print(setting);
    Serial.print(F(" to "));
    Serial.println(setting_value);
#endif

    StaticJsonDocument<JSON_FILE_SIZE> doc;
    String file_data;
    if (!getFile(file_data))
    {
        return false;
    }

    DeserializationError error = deserializeJson(doc, file_data);
    if (error)
    {
        Serial.print(F("SM: deserializeJson() failed: "));
        Serial.println(error.f_str());
        return false;
    }

    doc[setting] = setting_value;
    String out;
    serializeJsonPretty(doc, out);
    writeFile(out);
    return true;
}

#endif