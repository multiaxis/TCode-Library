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
 * @paragraph provides an interface for saving settings with SPIFFS
 */
class Settings
{
    const char *filepath;
    bool fileSystemMounted = false;
    bool foundFile = false;

public:
    /**
     * @brief Initializes the Settings class and mounts the SPIFFS filesystem
     * @param filepath Filepath in the SPIFFS filesystem to the relevent Settings Storage
    */
    bool init(const char *filepath);
    /**
     * @brief checks if the filesystem is mounted and filepath created
     * @return true if filesystem is mounted and filepath found
    */
    bool isMounted();
    /**
     * @brief checks if the given setting is in the settings file
     * @param setting C-String representation of the setting being accessed
     * @return true if setting is in the settings file
    */
    bool hasSetting(const char *setting);
    /**
     * Overwrites the stored settings with the values specified by the createConfigFile function
    */
    void reset();

    /**
     * @brief gets the value for a setting
     * @param setting C-String representation of the setting name being accessed
     * @param settingValue setting being obtained (is a generic type and does allow for type casting)
     * @return true if setting is found
    */
    template <class T>
    bool getSetting(const char *setting, T &settingValue);
    /**
     * @brief gets the value for a setting
     * @param setting C-String representation of the setting name being accessed
     * @param settingValue setting being stored (is a generic type and does allow for type casting)
     * @return true if setting is set
    */
    template <class T>
    bool setSetting(const char *setting, const T &settingValue);
    /**
     * @brief gets the system usage of the Settings system
     * @param out the struct used to store the settings details
     * @return returns false if the filesystem is not mounted
    */
    bool getSystemUsage(SettingsUsage& out);
    Settings() {}
    

private:
    /**
     * @brief gets the string representation of the file and puts it in the out string
     * @param out the string representation of the file
     * @return returns true if the file was read correctly
    */
    bool getFile(String &out);
    /**
     * @brief writes the string representation of the file
     * @param file content to be writen to file
     * @return returns true if file was written correctly
    */
    bool writeFile(const String &file);
    /**
     * @brief creates a string representation of the default config file
     * @paragraph is able to be overwritten by child classes inhereting from Settings
     * @param out the string representation of the file
    */
    virtual void createConfigFile(String &out);
    
    /**
     * @brief returns the size of the file stored in bytes
    */
    unsigned long getFileSize();
};

template <class T>
inline bool Settings::getSetting(const char *setting, T &settingValue)
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

    settingValue = doc[setting].as<T>();

    return true;
}

template <class T>
inline bool Settings::setSetting(const char *setting, const T &settingValue)
{

#ifdef DEBUG
    Serial.print(F("SM: setting setting "));
    Serial.print(setting);
    Serial.print(F(" to "));
    Serial.println(settingValue);
#endif

    StaticJsonDocument<JSON_FILE_SIZE> doc;
    String fileData;
    if (!getFile(fileData))
    {
        return false;
    }

    DeserializationError error = deserializeJson(doc, fileData);
    if (error)
    {
        Serial.print(F("SM: deserializeJson() failed: "));
        Serial.println(error.f_str());
        return false;
    }

    doc[setting] = settingValue;
    String out;
    serializeJson(doc, out);
    writeFile(out);
    return true;
}

#endif