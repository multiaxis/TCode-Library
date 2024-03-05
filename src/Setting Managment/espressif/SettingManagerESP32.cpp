// Setting-Management-Class-CPP v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 12/11/2023
// Please copy, share, learn, innovate, give attribution.
#include "SettingManagerESP32.h"



bool SettingManagerESP32::init()
{
    if (!SPIFFS.begin(true))
    {
        ESP_LOGE(SETTING_MANAGMENT_TAG,"an Error has occurred while mounting SPIFFS\n");
        return false;
    }
    else
    {
        ESP_LOGI(SETTING_MANAGMENT_TAG,"SPIFFS mounted correctly\n");
    }
    fileSystemMounted = true;
    foundFile = SPIFFS.exists(filepath);
    this->filepath = filepath;

    if (!foundFile)
    {
        foundFile = true;
        String newConfig = "{}";
        writeFile(newConfig);
        ESP_LOGI(SETTING_MANAGMENT_TAG,"setting's file not found creating settings file\n");
    }
    else
    {
        ESP_LOGI(SETTING_MANAGMENT_TAG,"setting's file found\n");
    }
    SettingsUsage usage;
    getSystemUsage(usage);
    ESP_LOGI(SETTING_MANAGMENT_TAG,"File System Initialised:\n================\n   System Info\n================\nSpace Available:%d\nSize of file:%d\nSize of SPIFFS:%d\n================\n",usage.spaceAvailable,usage.sizeOfFile,usage.spaceUsed);
    return true;
}

bool SettingManagerESP32::isMounted()
{
    return fileSystemMounted && foundFile;
}

bool SettingManagerESP32::keyInCache(const char* setting)
{
    size_t length = strlen(setting);
    unsigned long settingHash = str2hash(setting,length);
    for(size_t i = 0; i < cache.count(); i++)
    {
        TCodeTaggedDataContainer result;
        if(!cache.get(i,result))
        {
            continue;
        }

        if(result.getHash() == settingHash)
        {
            if(strcmp(result.getTag(),setting) == 0)
            {
                return true;
            }
        }
    }
    return false;
}

bool SettingManagerESP32::getValueFromCache(const char* setting,TCodeDataContainer& value)
{
    size_t length = strlen(setting);
    unsigned long settingHash = str2hash(setting,length);
    for(size_t i = 0; i < cache.count(); i++)
    {
        TCodeTaggedDataContainer result;
        if(!cache.get(i,result))
        {
            continue;
        }

        if(result.getHash() == settingHash)
        {
            if(strcmp(result.getTag(),setting) == 0)
            {
                value.setValue(*result.getDataContainer());
                return true;
            }
        }
    }
    return false;
}

bool SettingManagerESP32::setValueToCache(const char* setting,TCodeDataContainer value)
{
    if(!keyInCache(setting))
    {
        if(cache.full())
            cache.pop();
        TCodeTaggedDataContainer tagged(setting,value);
        cache.push(tagged);
        return true;
    }
    else
    {
        size_t length = strlen(setting);
        unsigned long settingHash = str2hash(setting,length);
        for(size_t i = 0; i < cache.count(); i++)
        {
            TCodeTaggedDataContainer result;
            if(!cache.get(i,result))
            {
                continue;
            }

            if(result.getHash() == settingHash)
            {
                if(strcmp(result.getTag(),setting) == 0)
                {
                    TCodeTaggedDataContainer tagged(setting,value);
                    cache.set(i,tagged);
                    return true;
                }
            }
        }
    }
    return false;
}

bool SettingManagerESP32::hasSetting(const char *setting)
{
    ESP_LOGI(SETTING_MANAGMENT_TAG,"checking cache for setting \"%s\"",setting);

    if(keyInCache(setting))
    {
        ESP_LOGI(SETTING_MANAGMENT_TAG,"key \"%s\" in cache",setting);
        return true;
    }

    String fileData;
    if (!getFile(fileData))
    {
        ESP_LOGE(SETTING_MANAGMENT_TAG,"could not load Settings file\n");
        return false;
    }
    StaticJsonDocument<DEFAULT_JSON_FILE_SIZE> doc;
    DeserializationError error = deserializeJson(doc, fileData);
    if (error)
    {
        ESP_LOGE(SETTING_MANAGMENT_TAG,"an Error has occurred with the json deserialisation Error Code:%s\n",error.f_str());
        return false;
    }

    if (!doc.containsKey(setting))
    {
        ESP_LOGI(SETTING_MANAGMENT_TAG,"setting \"%s\" not found\n",setting);
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
    ESP_LOGI(SETTING_MANAGMENT_TAG,"reading setting file \"%s\"\n",filepath);

    if (!isMounted())
    {
        ESP_LOGE(SETTING_MANAGMENT_TAG,"failed to read.\nfilesystem is not mounted.\nMounted Status:%d\nFile Status:%d\n",fileSystemMounted,foundFile);
        return false;
    }
    out = "";
    File file = SPIFFS.open(filepath);

    if (!file)
    {
        ESP_LOGE(SETTING_MANAGMENT_TAG,"file could not be opened.\n");
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
            ESP_LOGE(SETTING_MANAGMENT_TAG,"file is larger than %d cannot be parsed.\n",DEFAULT_JSON_FILE_SIZE);
            file.close();
            return false;
        }
    }
    ESP_LOGI(SETTING_MANAGMENT_TAG,"file read successfully.\n");
    file.close();
    return true;
}

bool SettingManagerESP32::writeFile(const String &fileData)
{
    ESP_LOGI(SETTING_MANAGMENT_TAG,"writing setting file \"%s\"\n",filepath);

    if (!isMounted())
    {
        ESP_LOGE(SETTING_MANAGMENT_TAG,"failed to write.\nfilesystem is not mounted.\nMounted Status:%d\nFile Status:%d\n",fileSystemMounted,foundFile);
        return false;
    }

    File file = SPIFFS.open(filepath, "w", true);

    if (!file)
    {
        ESP_LOGE(SETTING_MANAGMENT_TAG,"file could not be opened.\n");
        return false;
    }

    if(fileData.length() > DEFAULT_JSON_FILE_SIZE)
    {
        ESP_LOGE(SETTING_MANAGMENT_TAG,"file is larger than %d cannot be serialised.\n",DEFAULT_JSON_FILE_SIZE);
        return false;
    }

    for (unsigned int i = 0; i < fileData.length(); i++)
    {
        file.write(fileData[i]);
    }

    ESP_LOGI(SETTING_MANAGMENT_TAG,"file written successfully.\n");
    file.close();
    return true;
}

bool SettingManagerESP32::getSystemUsage(SettingsUsage &out)
{
    ESP_LOGI(SETTING_MANAGMENT_TAG,"getting system usage.\n");
    if (!isMounted()) 
    {
        ESP_LOGE(SETTING_MANAGMENT_TAG,"failed to get system usage.\nfilesystem is not mounted.\nMounted Status:%d\nFile Status:%d\n",fileSystemMounted,foundFile);
        return false;
    }
    out.sizeOfFile = getFileSize();
    out.spaceAvailable = SPIFFS.totalBytes();
    out.spaceUsed = SPIFFS.usedBytes();
    ESP_LOGI(SETTING_MANAGMENT_TAG,"got system usage.\n");
    return true;
}

unsigned long SettingManagerESP32::getFileSize()
{
    ESP_LOGI(SETTING_MANAGMENT_TAG,"getting file size.\n");
    if (!isMounted())
    {
        ESP_LOGE(SETTING_MANAGMENT_TAG,"failed to get file size.\nfilesystem is not mounted.\nMounted Status:%d\nFile Status:%d\n",fileSystemMounted,foundFile);
        return 0;
    }

    int size = 0;
    File file = SPIFFS.open(filepath);

    if (!file)
    {
        ESP_LOGE(SETTING_MANAGMENT_TAG,"file could not be opened.\n");
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
