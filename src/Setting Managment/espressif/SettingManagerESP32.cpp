// Setting-Management-Class-Hpp v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 12/11/2023
// Please copy, share, learn, innovate, give attribution.
#include "SettingManagerESP32.h"

bool SettingManagerESP32::init()
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
        String newConfig = "{}";
        writeFile(newConfig);
#ifdef DEBUG
        Serial.println(F("SM: file not found creating settings file"));

#endif
    }

#ifdef DEBUG
    Serial.println(F("SM: initialised"));
    SettingsUsage usage;
    getSystemUsage(usage);
    Serial.println("================");
    Serial.println("    File Sys    ");
    Serial.println("================");
    Serial.print("Space Available:");
    Serial.println(usage.spaceAvailable);
    Serial.print("Size:");
    Serial.println(usage.sizeOfFile);
    Serial.print("Used:");
    Serial.println(usage.spaceUsed);
    Serial.println("================");
#endif

    return true;
}

bool SettingManagerESP32::isMounted()
{
    return fileSystemMounted && foundFile;
}

bool SettingManagerESP32::hasSetting(const char *setting)
{
    StaticJsonDocument<DEFAULT_JSON_FILE_SIZE> doc;
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

void SettingManagerESP32::reset()
{
    String newConfig = "{}";
    writeFile(newConfig);
}

bool SettingManagerESP32::getFile(String &out)
{
    #ifdef DEBUG
    Serial.print(F("SM: reading file \""));
    Serial.print(filepath);
    Serial.println(F("\""));
    #endif

    if (!isMounted())
    {
        #ifdef DEBUG
        Serial.println(F("SM: Filesystem not mounted"));
        Serial.print(F("Mounted Status:"));
        Serial.println(fileSystemMounted);
        Serial.print(F("File Status:"));
        Serial.println(foundFile);
        #endif
        return false;
    }
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
        if (out.length() >= DEFAULT_JSON_FILE_SIZE)
        {
#ifdef DEBUG
            Serial.print(F("SM: File larger than "));
            Serial.println(DEFAULT_JSON_FILE_SIZE);
#endif
            file.close();
            return false;
        }
        #ifdef DEBUG
        Serial.print((char)current_byte);
        #endif
    }
    file.close();
    return true;
}

bool SettingManagerESP32::writeFile(const String &fileData)
{
#ifdef DEBUG
    Serial.print(F("SM: writing file \""));
    Serial.print(filepath);
    Serial.println(F("\""));
#endif

    if (!isMounted())
    {
        #ifdef DEBUG
        Serial.print(F("SM: Filesystem not mounted\""));
        Serial.print(filepath);
        Serial.print(F("\"\n Mounted Status:"));
        Serial.println(fileSystemMounted);
        Serial.print(F("\"\n File Status:"));
        Serial.println(foundFile);
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

    if(fileData.length() > DEFAULT_JSON_FILE_SIZE)
    {
        #ifdef DEBUG
        Serial.print(F("SM: Could not write file \""));
        Serial.print(filepath);
        Serial.print(F("\" "));
        Serial.println("Input value was too long");
        #endif
        return false;
    }

    for (unsigned int i = 0; i < fileData.length(); i++)
    {
        file.write(fileData[i]);
        #ifdef DEBUG
        Serial.print(fileData[i]);
        #endif
    }
    file.close();
    return true;
}

bool SettingManagerESP32::getSystemUsage(SettingsUsage &out)
{
    if (!isMounted())
        return false;
    out.sizeOfFile = getFileSize();
    out.spaceAvailable = SPIFFS.totalBytes();
    out.spaceUsed = SPIFFS.usedBytes();
    return true;
}

unsigned long SettingManagerESP32::getFileSize()
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
