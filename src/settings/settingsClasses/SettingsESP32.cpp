// Setting-Management-Class-CPP v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 12/11/2023
// Please copy, share, learn, innovate, give attribution.
#include "SettingsESP32.h"
#include "../../utils/TString.h"

namespace TCode::Settings
{

bool SettingsESP32::init()
{
    if (!SPIFFS.begin(true)) {
        ESP_LOGE(SETTING_MANAGMENT_TAG,"an Error has occurred while mounting SPIFFS\n");
        return false;
    } else {
        ESP_LOGI(SETTING_MANAGMENT_TAG,"SPIFFS mounted correctly\n");
    }

    fileSystemMounted = true;
    foundFile = SPIFFS.exists(filepath);
    this->filepath = filepath;
    if (!foundFile) {
        foundFile = true;
        reset();
        ESP_LOGI(SETTING_MANAGMENT_TAG,"setting's file not found creating settings file\n");
    } else {
        ESP_LOGI(SETTING_MANAGMENT_TAG,"setting's file found\n");
    }

    SettingsUsage usage;
    getSystemUsage(usage);
    ESP_LOGI(SETTING_MANAGMENT_TAG,"File System Initialised:\n================\n   System Info\n================\nSpace Available:%d\nSize of file:%d\nSize of SPIFFS:%d\n================\n",usage.spaceAvailable,usage.sizeOfFile,usage.spaceUsed);
    return true;
}

bool SettingsESP32::isMounted() {
    return fileSystemMounted && foundFile;
}

bool SettingsESP32::keyInCache(const char* setting)
{
    size_t length = strlen(setting);
    unsigned long settingHash = TString::getHash(setting, length);

    for(size_t i = 0; i < cache.size(); i++) {
        TaggedDataContainer result = cache[i];
        if(result.getHash() == settingHash)
            if(strcmp(result.getTag(),setting) == 0)
                return true;
    }

    return false;
}

bool SettingsESP32::getValueFromCache(const char* setting, DataContainer& value)
{
    size_t length = strlen(setting);
    unsigned long settingHash = TString::getHash(setting,length);

    for(size_t i = 0; i < cache.size(); i++) {
        TaggedDataContainer result = cache[i];
        if(result.getHash() == settingHash) {
            if(strcmp(result.getTag(), setting) == 0) {
                DataContainer containedValue;
                if(result.getValue(containedValue))
                {
                    value = containedValue;
                }
                return true;
            }
        }
    }

    return false;
}

bool SettingsESP32::setValueToCache(const char* setting, DataContainer value) {
    if(!keyInCache(setting)) {
        if(cache.size() == DEFAULT_SETTING_CACHE_SIZE)
            cache.pop_front();
            
        cache.push_back(TaggedDataContainer(setting,value));
        return true;
    } else {
        size_t length = strlen(setting);
        unsigned long settingHash = TString::getHash(setting,length);

        for(size_t i = 0; i < cache.size(); i++) {
            TaggedDataContainer result = cache[i];
            if(result.getHash() == settingHash) {
                if(strcmp(result.getTag(),setting) == 0){
                    cache[i] = TaggedDataContainer(setting,value);
                    return true;
                }
            }
        }
    }

    return false;
}

bool SettingsESP32::hasSetting(const char *setting) {
    ESP_LOGI(SETTING_MANAGMENT_TAG,"checking cache for setting \"%s\"",setting);

    if(keyInCache(setting)) {
        ESP_LOGI(SETTING_MANAGMENT_TAG,"key \"%s\" in cache",setting);
        return true;
    }

    String fileData;
    if (!getFile(fileData)) {
        ESP_LOGE(SETTING_MANAGMENT_TAG,"could not load Settings file\n");
        return false;
    }

    StaticJsonDocument<DEFAULT_JSON_FILE_SIZE> doc;
    DeserializationError error = deserializeJson(doc, fileData);
    if (error) {
        ESP_LOGE(SETTING_MANAGMENT_TAG,"an Error has occurred with the json deserialisation Error Code:%s\n",error.f_str());
        return false;
    }

    if (!doc.containsKey(setting)) {
        ESP_LOGI(SETTING_MANAGMENT_TAG,"setting \"%s\" not found\n",setting);
        return false;
    }

    return true;
}

void SettingsESP32::reset() {
    String newConfig = "{}";
    writeFile(newConfig);
}

bool SettingsESP32::getFile(String &out) {
    ESP_LOGI(SETTING_MANAGMENT_TAG,"reading setting file \"%s\"\n",filepath);

    if (!isMounted()) {
        ESP_LOGE(SETTING_MANAGMENT_TAG,"failed to read.\nfilesystem is not mounted.\nMounted Status:%d\nFile Status:%d\n",fileSystemMounted,foundFile);
        return false;
    }

    out = "";
    File file = SPIFFS.open(filepath);
    if (!file) {
        ESP_LOGE(SETTING_MANAGMENT_TAG,"file could not be opened.\n");
        return false;
    }

    while (file.available()) {
        uint8_t currentByte = file.read();
        if (currentByte == '\n')
            continue;

        out += (char)currentByte;
        if (out.length() >= DEFAULT_JSON_FILE_SIZE) {
            ESP_LOGE(SETTING_MANAGMENT_TAG,"file is larger than %d cannot be parsed.\n",DEFAULT_JSON_FILE_SIZE);
            file.close();
            return false;
        }
    }

    ESP_LOGI(SETTING_MANAGMENT_TAG,"file read successfully.\n");
    file.close();
    return true;
}

bool SettingsESP32::writeFile(const String &fileData) {
    ESP_LOGI(SETTING_MANAGMENT_TAG,"writing setting file \"%s\"\n",filepath);

    if (!isMounted()) {
        ESP_LOGE(SETTING_MANAGMENT_TAG,"failed to write.\nfilesystem is not mounted.\nMounted Status:%d\nFile Status:%d\n",fileSystemMounted,foundFile);
        return false;
    }

    File file = SPIFFS.open(filepath, "w", true);
    if (!file) {
        ESP_LOGE(SETTING_MANAGMENT_TAG,"file could not be opened.\n");
        return false;
    }

    if(fileData.length() > DEFAULT_JSON_FILE_SIZE) {
        ESP_LOGE(SETTING_MANAGMENT_TAG,"file is larger than %d cannot be serialised.\n",DEFAULT_JSON_FILE_SIZE);
        return false;
    }

    for (unsigned int i = 0; i < fileData.length(); i++)
        file.write(fileData[i]);

    ESP_LOGI(SETTING_MANAGMENT_TAG,"file written successfully.\n");
    file.close();
    return true;
}

bool SettingsESP32::getSystemUsage(SettingsUsage &out) {
    ESP_LOGI(SETTING_MANAGMENT_TAG,"getting system usage.\n");
    if (!isMounted()) {
        ESP_LOGE(SETTING_MANAGMENT_TAG,"failed to get system usage.\nfilesystem is not mounted.\nMounted Status:%d\nFile Status:%d\n",fileSystemMounted,foundFile);
        return false;
    }

    out.sizeOfFile = getFileSize();
    out.spaceAvailable = SPIFFS.totalBytes();
    out.spaceUsed = SPIFFS.usedBytes();
    ESP_LOGI(SETTING_MANAGMENT_TAG,"got system usage.\n");
    return true;
}

unsigned long SettingsESP32::getFileSize() {
    ESP_LOGI(SETTING_MANAGMENT_TAG,"getting file size.\n");
    if (!isMounted()) {
        ESP_LOGE(SETTING_MANAGMENT_TAG,"failed to get file size.\nfilesystem is not mounted.\nMounted Status:%d\nFile Status:%d\n",fileSystemMounted,foundFile);
        return 0;
    }

    int size = 0;
    File file = SPIFFS.open(filepath);
    if (!file) {
        ESP_LOGE(SETTING_MANAGMENT_TAG,"file could not be opened.\n");
        return 0;
    }

    while (file.available()) {
        file.read();
        size += 1;
    }

    file.close();
    return size;
}

template <typename T>
inline bool SettingsESP32::getSettingTemplated(const char *setting, T &settingValue) {
#ifdef DEBUG
    Serial.print("SM: Checking Cache For setting \"");
    Serial.print(setting);
    Serial.println("\"");
#endif
    if(keyInCache(setting)) {
#ifdef DEBUG
        Serial.println("SM: value found in cache:");
#endif
        DataContainer result;
        if(getValueFromCache(setting, result))
            return result.getValue<T>(settingValue);
    }



#ifdef DEBUG
    Serial.print(F("SM: getting setting "));
    Serial.println(setting);
#endif
    String file_data;
    if (!getFile(file_data)) {
#ifdef DEBUG
        Serial.println(F("SM: error getting file data "));
#endif
        return false;
    }

    //Serial.println(file_data);

    StaticJsonDocument<DEFAULT_JSON_FILE_SIZE> doc;
    DeserializationError error = deserializeJson(doc, file_data);
    if (error) {
#ifdef DEBUG
        Serial.print(F("SM: deserializeJson() failed: "));
        Serial.println(error.f_str());
#endif
        return false;
    }

#ifdef DEBUG_VERBOSE
    Serial.print(F("SM: keys "));
    JsonObject documentRoot = doc.as<JsonObject>();
    for (JsonPair keyValue : documentRoot) {
        Serial.print(F("\""));
        Serial.print(keyValue.key().c_str());
        Serial.print("\":\"");
        Serial.print(keyValue.value().as<String>());
        Serial.println(F("\""));
    }
#endif

    if (!doc.containsKey(setting)) {
#ifdef DEBUG
        Serial.println(F("SM: key not found"));
#endif
        return false;
    }

    settingValue = doc[setting].as<T>();
    DataContainer settingValueContainer(settingValue);
    setValueToCache(setting,settingValueContainer);
    return true;
}

template <typename T>
inline bool SettingsESP32::setSettingTemplated(const char *setting, const T &settingValue) {
#ifdef DEBUG
    Serial.print(F("SM: setting setting "));
    Serial.print(setting);
    Serial.print(F(" to "));
    Serial.println(settingValue);
#endif

    String fileData;
    if (!getFile(fileData)) {
        return false;
    }

    DataContainer settingValueContainer(settingValue);
    setValueToCache(setting,settingValueContainer);

    StaticJsonDocument<DEFAULT_JSON_FILE_SIZE> doc;
    DeserializationError error = deserializeJson(doc, fileData);
    if (error) {
#ifdef DEBUG
        Serial.print(F("SM: deserializeJson() failed: "));
        Serial.println(error.f_str());
#endif
        return false;
    }

    doc[setting] = settingValue;
    String out;
    serializeJson(doc, out);
    //Serial.println(out);
    writeFile(out);
    return true;
}

}