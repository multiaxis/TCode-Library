#pragma once
#ifndef TCODE_HPP
#define TCODE_HPP
#include "Setting Managment/SettingManagement.hpp"
#include "Parsing/TCodeParser.h"
#include "Axis Management/TCodeAxis.h"
#include "Utils/TCodeBuffer.h"
#include "Constants and Enums/TCodeConstants.h"
#include "Constants and Enums/TCodeEnums.h"

/*

TCode -
    - Input
    - Buffer
    - Run Commands
    - Store Axis
Axis - Done needs testing
Setting Management - Done needs testing - tested
Parser - Done needs testing

*/

#define CURRENT_TCODE_VERSION "TCode v0.4";
#define DEFAULT_FIRMWARE_NAME "TCode";

const int MAX_AXIS_COUNT = 10;
const int MAX_COMMAND_BUFFER_LENGTH_COUNT = 127;
const int MAX_INPUT_BUFFER_LENGTH_COUNT = 127;
const int MAX_OUTPUT_BUFFER_LENGTH_COUNT = 127;

class TCode
{
public:

    TCode();
    TCode(const char* firmware);
    TCode(const char* firmware,const char* tcode_version);

    void inputByte(const byte input);      // Function to read off individual byte as input to the command buffer
    void inputChar(const char input);      // Function to read off individual char as input to the command buffer
    void inputString(const String &input); // Function to take in a string as input to the command buffer
    void inputCString(const char *&input); // Function to take in a c_string as input to the command buffer
    void clearBuffer();                    // Function to clear the input buffer;

    bool registerAxis(TCodeAxis *axis);

    void axisWrite(const TCode_ChannelID &id, const int magnitude, const TCode_Axis_Extention_Type extentionValue = TCode_Axis_Extention_Type::Time, const long extMagnitude = 0, const TCode_Axis_Ramp_Type rampType = TCode_Axis_Ramp_Type::Linear);
    void axisWrite(const char *name, const int magnitude, const TCode_Axis_Extention_Type extentionValue = TCode_Axis_Extention_Type::Time, const long extMagnitude = 0, const TCode_Axis_Ramp_Type rampType = TCode_Axis_Ramp_Type::Linear);
    int axisRead(const TCode_ChannelID &channel_id);
    int axisRead(const char *name);

    void stop();

    size_t available();
    char getChar();
    size_t getChar(char *buffer, const size_t length);

private:
    const char* filepath;
    const char* firmwareVersion;
    const char* tcodeVersion;
    Settings settingManager;
    TCodeBuffer<TCodeAxis *, MAX_AXIS_COUNT> axisBuffer;
    TCodeBuffer<char, MAX_OUTPUT_BUFFER_LENGTH_COUNT> outputBuffer;
    TCodeBuffer<char, MAX_INPUT_BUFFER_LENGTH_COUNT> inputBuffer;

    TCodeAxis* getAxisFromName(const char* name);
    TCodeAxis* getAxisFromID(const TCode_ChannelID &id);
    void executeNextBufferCommand();
    void readCommand(unsigned char* buffer, size_t length);

    void runAxisCommand(TCode_Axis_Command &command);
    void runDeviceCommand(TCode_Device_Command &command);
    void runSetupCommand(TCode_Setup_Command &command);
    void runExternalCommand(TCode_External_Command &command);

    void setSaveValues(TCode_ChannelID& id, unsigned int min, unsigned int max);

    void printSavedAxisValues();

    void print(const char *value);
    void print(const __FlashStringHelper *value);
    void print(const String &value);
    void println(const char *value);
    void println(const __FlashStringHelper *value);
    void println(const String &value);
};

#endif