#include "SettingManagement.hpp"


bool Settings::init(const char *filepath)
{
    if (!SPIFFS.begin(true))
    {
#ifdef DEBUG
        Serial.println(F("SM: an Error has occurred while mounting SPIFFS"));
#endif
        return false;
    }
    file_system_mounted = true;
    found_file = SPIFFS.exists(filepath);

    if (!found_file)
    {

        String newConfig;
        createConfigFile(newConfig);
        writeFile(newConfig);
#ifdef DEBUG
        Serial.println(F("SM: file not found creating settings file"));
        outputSystemUsage();
#endif
    }
    return true;
}

bool Settings::isMounted()
{
    return file_system_mounted && found_file;
}

bool Settings::hasSetting(const char *setting)
{
    StaticJsonDocument<JSON_FILE_SIZE> doc;
    String file_data;
    if (!getFile(file_data))
    {
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

    if (!doc.containsKey(setting))
    {
        return false;
    }

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
        if (current_byte == ' ')
            continue;
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

bool Settings::writeFile(const String &file_data)
{
    if (!isMounted())
        return false;
    File file = SPIFFS.open(filepath, "w", true);

    if (!file)
    {
#ifdef DEBUG
        Serial.print(F("SM: Could not find file \""));
        Serial.print(filepath);
        Serial.println(F("\""));
#endif
        return false;
    }

    for (unsigned int i = 0; i < file_data.length(); i++)
    {
        file.write(file_data[i]);
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
        return false;
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

void Settings::outputSystemUsage()
{
    Serial.println(F("Settings Manager:"));
    Serial.print(F("Used by settings file:"));
    Serial.println(getFileSize());
    Serial.print(F("Current Total Bytes:"));
    Serial.println(SPIFFS.totalBytes());
    Serial.print(F("Current Bytes Used:"));
    Serial.println(SPIFFS.usedBytes());
}
