// TCode-Class-h v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 10/09/2023
// Please copy, share, learn, innovate, give attribution.
#pragma once
#ifndef TCODE_HPP
#define TCODE_HPP
#include "Setting Managment/SettingManagement.h"
#include "Parsing/TCodeParser.h"
#include "Axis Management/TCodeAxis.h"
#include "Utils/TCodeBuffer.h"
#include "Constants and Enums/TCodeConstants.h"
#include "Constants and Enums/TCodeEnums.h"

#define CURRENT_TCODE_VERSION "TCode v0.4"
#define DEFAULT_FIRMWARE_NAME "TCode"
#define DEFAULT_FILE_NAME "/spiffs/TCode.dat"

const int MAX_AXIS_COUNT = 10;
const int MAX_COMMAND_BUFFER_LENGTH_COUNT = 127;
const int MAX_INPUT_BUFFER_LENGTH_COUNT = 127;
const int MAX_OUTPUT_BUFFER_LENGTH_COUNT = 127;

class TCode
{
public:

    /**
     * @brief default constructor for the TCode Class
     */
    TCode();
    /**
     * @brief constructor for the TCode class
     * @param firmware the name of firmware
     */
    TCode(const char *firmware);
     /**
     * @brief constructor for the TCode class
     * @param firmware the name of firmware represented as a c-string
     * @param tcodeVersion the name of tcode version represented as a c-string
     */
    TCode(const char *firmware, const char *tcodeVersion);
    /**
     * @brief constructor for the TCode class
     * @param firmware the name of firmware represented as a c-string
     * @param tcodeVersion the name of tcode version represented as a c-string
     * @param filepath the filepath used by the settings file in SPIFFS represented by a c-string
     */
    TCode(const char *firmware, const char *tcodeVersion, const char *filepath);

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
     * @brief function to register a TCodeAxis with the TCode class
     * @param axis TCodeAxis pointer to be added to the TCode class to be processed
     * @return returns false if axis could not be registered
     */
    bool registerAxis(TCodeAxis *axis);

    /**
     * @brief function to write to a specified axis
     * @param id channel ID to write to
     * @param targetValue the target value for this axis
     * @param extentionValue the extention type for the axis e.g. Time,Speed
     * @param extMagnitude the value for the extention type
     * @param rampType the ramp type
     */
    void axisWrite(const TCode_ChannelID &id, const int targetValue, const TCode_Axis_Extention_Type extentionValue = TCode_Axis_Extention_Type::Time, const long extMagnitude = 0, const TCode_Axis_Ramp_Type rampType = TCode_Axis_Ramp_Type::Linear);
    
    /**
     * @brief function to write to a specified axis
     * @param name name of the channel to write to
     * @param targetValue the target value for this axis
     * @param extentionValue the extention type for the axis e.g. Time,Speed
     * @param extMagnitude the value for the extention type
     * @param rampType the ramp type
     */
    void axisWrite(const char *name, const int targetValue, const TCode_Axis_Extention_Type extentionValue = TCode_Axis_Extention_Type::Time, const long extMagnitude = 0, const TCode_Axis_Ramp_Type rampType = TCode_Axis_Ramp_Type::Linear);
    
    /**
     * @brief function to read the position of a specified axis
     * @param id channel ID to read the current position from
     * @returns current position or -1 if position could not be read
     */
    int axisRead(const TCode_ChannelID &channel_id);

    /**
     * @brief function to read the position of a specified axis
     * @param name name of the channel to read the current position from
     * @returns current position or -1 if position could not be read
     */
    int axisRead(const char *name);

    /**
     * @brief stops all axis movement at its current position (sets vibration channels to 0)
     */
    void stop();

    /**
     * @brief returns current available bytes in output buffer
     */
    size_t available();

    /**
     * @brief returns next byte in output buffer (returns '\0' if no bytes present)
     */
    char getChar();

    /**
     * @brief returns bytes in output buffer to an external buffer
     * @return returns length of bytes put into the provided buffer
     */
    size_t getChar(char *buffer, const size_t length);


    /**
     * @brief returns current available bytes in external command output buffer
     */
    size_t externalAvailable();

    /**
     * @brief returns next byte in external command output buffer (returns '\0' if no bytes present)
     */
    char getExternalChar();

    /**
     * @brief returns bytes in external command output buffer to an external buffer
     * @return returns length of bytes put into the provided buffer
     */
    size_t getExternalChar(char *buffer, const size_t length);



    /**
     * @brief gets the current instance of the settings manager
     * @return returns the current Settings manager
     */
    Settings *getSettingManager();

private:
    const char *filepath;
    const char *firmwareVersion;
    const char *tcodeVersion;
    Settings settingManager;
    TCodeBuffer<TCodeAxis *, MAX_AXIS_COUNT> axisBuffer;
    TCodeBuffer<char, MAX_OUTPUT_BUFFER_LENGTH_COUNT> outputBuffer;
    TCodeBuffer<char, MAX_OUTPUT_BUFFER_LENGTH_COUNT> externalCommandBuffer;
    TCodeBuffer<char, MAX_INPUT_BUFFER_LENGTH_COUNT> inputBuffer;

    TCodeAxis *getAxisFromName(const char *name);
    TCodeAxis *getAxisFromID(const TCode_ChannelID &id);
    void executeNextBufferCommand();
    void readCommand(unsigned char *buffer, size_t length);

    void runAxisCommand(TCode_Axis_Command &command);
    void runDeviceCommand(TCode_Device_Command &command);
    void runSetupCommand(TCode_Setup_Command &command);
    void runExternalCommand(TCode_External_Command &command);

    void setSaveValues(TCode_ChannelID &id, unsigned int min, unsigned int max);

    void printSavedAxisValues();

    void print(const char *value);
    void print(const __FlashStringHelper *value);
    void print(const String &value);
    void println(const char *value);
    void println(const __FlashStringHelper *value);
    void println(const String &value);
};

#endif