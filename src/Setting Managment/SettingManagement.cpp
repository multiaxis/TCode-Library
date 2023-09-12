// Setting-Management-Class-Cpp v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 9/09/2023
// Please copy, share, learn, innovate, give attribution.

#ifndef SETTING_MANAGMENT_CPP
#define SETTING_MANAGEMET_CPP
#include "SettingManagement.h"

bool Settings::init(const char *filepath)
{
    if (!SPIFFS.begin(true))
    {
#ifdef DEBUG
        Serial.println(F("SM: an Error has occurred while mounting SPIFFS"));
#endif
        return false;
    }
    else
    {
#ifdef DEBUG
        Serial.println(F("SM: SPIFFS mounted correctly"));
#endif
    }
    fileSystemMounted = true;
    foundFile = SPIFFS.exists(filepath);
    this->filepath = filepath;

#ifdef DEBUG
    if (foundFile)
        Serial.println(F("SM: setting's file found"));
    else
        Serial.println(F("SM: setting's file not found"));
#endif

    if (!foundFile)
    {
        foundFile = true;
        String newConfig;
        createConfigFile(newConfig);
        writeFile(newConfig);
#ifdef DEBUG
        Serial.println(F("SM: file not found creating settings file"));

#endif
    }

#ifdef DEBUG
    Serial.println(F("SM: initialised"));
    outputSystemUsage();
#endif

    return true;
}

bool Settings::isMounted()
{
    return fileSystemMounted && foundFile;
}

bool Settings::hasSetting(const char *setting)
{
    StaticJsonDocument<JSON_FILE_SIZE> doc;
    String fileData;
    if (!getFile(fileData))
    {
        return false;
    }

    DeserializationError error = deserializeJson(doc, fileData);
    if (error)
    {
#ifdef DEBUG
        Serial.print(F("SM: deserializeJson() failed: "));
        Serial.println(error.f_str());
#endif
        return false;
    }

    if (!doc.containsKey(setting))
    {
        return false;
    }

    return true;
}

void Settings::reset()
{
    String newConfig;
    createConfigFile(newConfig);
    writeFile(newConfig);
}

bool Settings::getSystemUsage(SettingsUsage &out)
{
    if (!isMounted())
        return false;
    out.sizeOfFile = getFileSize();
    out.spaceAvailable = SPIFFS.totalBytes();
    out.spaceUsed = SPIFFS.usedBytes();
    return true;
}

bool Settings::getFile(String &out)
{
    if (!isMounted())
        return false;
    out = "";
    File file = SPIFFS.open(filepath);

    if (!file)
    {
#ifdef DEBUG
        Serial.print(F("SM: Could not find file \""));
        Serial.print(filepath);
        Serial.println(F("\""));
#endif
        return false;
    }

    while (file.available())
    {
        uint8_t current_byte = file.read();
        if (current_byte == '\n')
            continue;
        out += (char)current_byte;
        if (out.length() >= JSON_FILE_SIZE)
        {
#ifdef DEBUG
            Serial.print(F("SM: File larger than "));
            Serial.println(JSON_FILE_SIZE);
#endif
            file.close();
            return false;
        }
    }
    file.close();
    return true;
}

bool Settings::writeFile(const String &fileData)
{
#ifdef DEBUG
    Serial.print(F("SM: writing file \""));
    Serial.print(filepath);
    Serial.println(F("\""));
#endif

    if (!isMounted())
    {
#ifdef DEBUG
        Serial.println(F("SM: file system not mounted"));
#endif
        return false;
    }

    File file = SPIFFS.open(filepath, "w", true);

    if (!file)
    {
#ifdef DEBUG
        Serial.print(F("SM: Could not write file \""));
        Serial.print(filepath);
        Serial.println(F("\""));
#endif
        return false;
    }

    for (unsigned int i = 0; i < fileData.length(); i++)
    {
        file.write(fileData[i]);
    }
    file.close();
    return true;
}

void Settings::createConfigFile(String &out)
{
    out = "{\"DEFAULT CONFIG\":\"TESTING\"}";
}

unsigned long Settings::getFileSize()
{
    if (!isMounted())
        return 0;
    int size = 0;
    File file = SPIFFS.open(filepath);

    if (!file)
    {
#ifdef DEBUG
        Serial.print(F("SM: Could not find file \""));
        Serial.print(filepath);
        Serial.println(F("\""));
#endif
        return 0;
    }

    while (file.available())
    {
        file.read();
        size += 1;
    }
    file.close();
    return size;
}

#endif