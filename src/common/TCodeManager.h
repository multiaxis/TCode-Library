// TCode-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 10/09/2023
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include <Arduino.h>
#include <vector>
#include <deque>
#include "../internal/TCodeEnums.h"
#include "../internal/axis/TAxis.h"
#include "../internal/input/TInputBase.h"
#include "../common/settings/ISettings.h"

#define DEFAULT_TCODE_VERSION "TCode v0.4"
#define DEFAULT_FIRMWARE_NAME "TCode"
#define DEFAULT_FILE_NAME "/spiffs/TCode.dat"

namespace TCode {
    class TInputBase;
}

using namespace std;
using namespace TCode;

class TCodeManager
{
public:
    /**
     * @brief constructor for the TCode class
     * @param firmware the name of firmware represented as a c-string
     * @param tcodeVersion the name of tcode version represented as a c-string
     */
    TCodeManager(const char *firmware = DEFAULT_FIRMWARE_NAME, const char *tcodeVersion = DEFAULT_TCODE_VERSION);

    /**
     * @brief function to input a byte into the input buffer
     * @param input byte to be inputted
     */
    void inputByte(const byte input);

    /**
     * @brief function to input a char into the input buffer
     * @param input char to be inputted
     */
    void inputChar(const char input);

    /**
     * @brief function to input a string into the input buffer
     * @param input string to be inputted
     */
    void inputString(const String &input);

    /**
     * @brief function to input a c-string into the input buffer
     * @param input c-string to be inputted
     */
    void inputCString(const char *&input);

    /**
     * @brief function to clear the input buffer
     */
    void clearBuffer();

    /**
     * @brief function to register a UnbufferedAxis with the TCode class
     * @param axis UnbufferedAxis pointer to be added to the TCode class to be processed
     * @return returns false if axis could not be registered
     */
    bool registerAxis(const char *name, ChannelID channel, float defaultValue);

    /**
     * @brief function to write to a specified axis
     * @param id channel ID to write to
     * @param targetValue the target value for this axis
     * @param extentionValue the extention type for the axis e.g. TIME,SPEED
     * @param extMagnitude the value for the extention type
     * @param rampType the ramp type
     */
    void axisWrite(const ChannelID &id, const AxisData &data);

    /**
     * @brief function to write to a specified axis
     * @param name name of the channel to write to
     * @param targetValue the target value for this axis
     * @param extentionValue the extention type for the axis e.g. TIME,SPEED
     * @param extMagnitude the value for the extention type
     * @param rampType the ramp type
     */
    void axisWrite(const char *name, const AxisData &data);

    /**
     * @brief function to read the position of a specified axis
     * @param id channel ID to read the current position from
     * @returns current position or -1 if position could not be read
     */
    float axisRead(const ChannelID &channelID);

    /**
     * @brief function to read the position of a specified axis
     * @param name name of the channel to read the current position from
     * @returns current position or -1 if position could not be read
     */
    float axisRead(const char *name);

    /**
     * @brief function to read the last command time of a specified axis
     * @param channelID id of the channel to read the last command time from
     * @returns last command time or -1 if position could not be read
     */
    unsigned long axisLastCommandTime(const ChannelID &channelID);
    
    /**
     * @brief function to read the last command time of a specified axis
     * @param name name of the channel to read the last command time from
     * @returns last command time or -1 if position could not be read
     */
    unsigned long axisLastCommandTime(const char *name);

    /**
     * @brief Main update function for handling interfaces
     */
    void update();

    /**
     * @brief Registers an interface to the tcode class 
     */
    void registerButton(unsigned int pin, const char* name, void (*callback)() = nullptr);

    /**
     * @brief stops all axis movement at its current position (sets vibration channels to 0)
     */
    void stop();

    /**
     * @brief sets the current instance of the settings manager and initialises it
     * @param settings pointer to the settings object
     */
    void setSettingManager(ISettings *settings);

    /**
     * @brief sets the output printing stream
     * @param stream pointer to the print object
     */
    void setOutputStream(Print *stream);

    /**
     * @brief gets the current firmware ID
     */
	const char * getFirmwareID() { return firmwareVersion; }

    /**
     * @brief gets the current TCode Version
     */
    const char * getTCodeVersion() { return tcodeVersion; }

    /**
     * @brief sets the current overwrite method
     * @param set bool value for is overwrite method should be used default is true
     */
    void useOverwriteMethod(bool set) { useOverwrite = set; }

private:
    bool useOverwrite = true;
    
    const char *filepath;
    const char *firmwareVersion;
    const char *tcodeVersion;
    ISettings *settingManager;
    vector<TAxis> registeredAxes;
    vector<TInputBase*> registeredInputs;
    vector<AxisCommand> axisCommandBuffer;
    deque<char> inputBuffer;
    Print* outputStream;
    

    TAxis *getAxisFromName(const char *name);
    TAxis *getAxisFromID(const ChannelID &id);
    void executeNextBufferCommand();
    void readCommand(char *buffer, size_t length);

    void runAxisCommand(AxisCommand &command);
    void runDeviceCommand(DeviceCommand &command);
    void runSetupCommand(SetupCommand &command);

    void setSaveValues(ChannelID &id, float min, float max,uint8_t min_log = 4, uint8_t max_log = 4);

    void printSavedAxisValues();

    void print(const char value);
    void print(const char *value);
    void print(const __FlashStringHelper *value);
    void print(const String &value);
    void println(const char value);
    void println(const char *value);
    void println(const __FlashStringHelper *value);
    void println(const String &value);
};