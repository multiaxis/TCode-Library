// TCode-Parser-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 26/06/2022
// Please copy, share, learn, innovate, give attribution.
#pragma once
#ifndef TCODE_PARSER_H
#define TCODE_PARSER_H
#include "esp_log.h"
#include <Arduino.h>
#include "../Constants and Enums/TCodeConstants.h"
#include "../Constants and Enums/TCodeEnums.h"
#include "../Utils/TCodeBuffer.h"
#include "../Utils/TCodeCStringUtils.h"

/**
 * @brief Wrapper for parsing commands from TCode
 */
class TCodeParser
{
public:
    /**
     * @brief Checks if the given ID is in a valid range compared to a max channel input
     * @param type The channel type Linear,Rotation,Vibration,Auxiliary
     * @param channel The channel number if the ID
     * @param maxChannel the max channel to check against
     */
    static bool idValidRange(const TCode_Channel_Type type, const uint8_t channel, const uint8_t maxChannel);

    /**
     * @brief Checks if the given id is in a valid range compared to a max channel input
     * @param id The ID to check
     * @param maxChannel the max channel to check against
     */
    static bool idValidRange(const TCode_ChannelID id, const uint8_t maxChannel);

    /**
     * @brief Checks if the inputted type and channel number are valid
     * @param type the TCode Channel Type
     * @param channel the Channel number
     * @returns returns true if the type and channel number are within accepted ranges and values 0-9 on channel number and a valid channel type
     */
    static bool idValid(const TCode_Channel_Type type, const uint8_t channel);

    /**
     * @brief Checks if the inputted type and channel number are valid
     * @param id Is the combined form of TCode_Channel_Type and the channel number
     * @returns returns true if the type and channel number are within accepted ranges and values 0-9 on channel number and a valid channel type
     */
    static bool idValid(const TCode_ChannelID id);

    /**
     * @brief For a given ID returns the string representation of the ID
     * @param buffer output buffer
     * @param length length of buffer
     * @param id Is the combined form of TCode_Channel_Type and the channel number
     * @returns The length of chars used in the buffer
     */
    static int getStrfromID(char *buffer, const size_t length, const TCode_ChannelID &id);

    /**
     * @brief For a given ID returns the string representation of the ID
     * @param id Is the combined form of TCode_Channel_Type and the channel number
     * @param out The Output String
     */
    static void getStrfromID(const TCode_ChannelID &id, String &out);

    /**
     * @brief Takes a channel type and channel number and creates a Channel ID
     * @param type the TCode Channel Type
     * @param channel the Channel number
     * @returns returns a TCode_ChannelID from the channel type and channel number
     */
    static TCode_ChannelID constructID(const TCode_Channel_Type type, const uint8_t channel);

    /**
     * @brief Gets the Extention type from a string at the index specified
     * @param buffer string to be processed
     * @param length length of buffer
     * @param index starting index of the char to be processed
     * @returns returns a TCode Axis Extention Type used in the Axis to work out if an extention means Time or Speed
     */
    static TCode_Axis_Extention_Type getExtentionTypeFromStr(char *buffer, const size_t length, size_t &index);
    /**
     * @brief Gets the Ramp type from a string at the index specified
     * @param buffer string to be processed
     * @param length length of buffer
     * @param index starting index of the char to be processed
     * @param rampType returning ramp type
     * @returns returns a true if ramp type was parsed correctly
     */
    static bool getRampTypeFromStr(char *buffer, const size_t length, size_t &index, TCode_Axis_Ramp_Type &rampType);

    /**
     * @brief Gets the ID from an inputted string from a given index
     * @param buffer string to be processed
     * @param length length of buffer
     * @param index starting index
     * @returns returns a TCode Channel ID found at the location at the starting index
     */
    static TCode_ChannelID getIDFromStr(char *buffer, const size_t length, size_t &index);

    /**
     * @brief Returns the type of command provided by the input string
     * @param buffer string to be processed
     * @param length length of buffer
     * @returns a TCode Command Type e.g. Axis, Device, Setup if it is not a valid command None is returned
     */
    static TCode_Command_Type getCommandType(char *buffer, const size_t length, size_t index);

    /**
     * @brief Parses the next command out of a buffer and puts it into a provided char buffer
     * @param inputBuffer buffer for command to be parsed from
     * @param buffer buffer for command to go into
     * @param length length of buffer
     * @param index starting index of the char to be processed
     * @returns a TCode Command Type e.g. Axis, Device, Setup if it is not a valid command None is returned
     */
    static size_t getNextCommand(TCodeBuffer<char> *inputBuffer, char *buffer, size_t buffer_length);

    /**
     * @brief Parses the next command out of a buffer and puts it into a provided char buffer
     * @param inputBuffer buffer for command to be parsed from
     * @param length length of input buffer
     * @param index starting index
     * @param outbuffer buffer for command to go into
     * @param outBufferLength length of buffer
     * @returns a TCode Command Type e.g. Axis, Device, Setup if it is not a valid command None is returned
     */
    static size_t getNextCommand(char *inputBuffer, const size_t length, const size_t index, char *outbuffer, const size_t outBufferLength);

    /**
     * @brief Parses an Axis Command
     * @param buffer string to be processed
     * @param length length of buffer
     * @param out a TCode Axis command struct so that the command can be executed easier
     */
    static bool parseAxisCommand(char *buffer, const size_t length, TCode_Axis_Command &out);

    /**
     * @brief Parses the next command out of a buffer and puts it into a provided char buffer
     * @param inputBuffer buffer for command to be parsed from
     * @param length length of input buffer
     * @param index starting index
     * @param extentionType returning extention type
     * @param commandExtention returning extendion value
     * @returns returns a true if axis extention was parsed correctly
     */
    static bool parseAxisExtention(char *buffer, const size_t length, size_t &index, TCode_Axis_Extention_Type &extentionType, long &commandExtention);
    
    /**
     * @brief Parses the next command out of a buffer and puts it into a provided char buffer
     * @param inputBuffer buffer for command to be parsed from
     * @param length length of input buffer
     * @param index starting index
     * @param rampType returning ramp type
     * @param rampIn returning ramp in data
     * @param rampOut returning ramp out data
     * @returns returns a true if axis ramp was parsed correctly
     */
    static bool bool parseAxisRamp(char *buffer, const size_t length, size_t &index, TCode_Axis_Ramp_Type &rampType, TCode_Axis_Ramp_Data &rampIn, TCode_Axis_Ramp_Data &rampOut);

    /**
     * @brief Parses a Device Command
     * @param buffer string to be processed
     * @param length length of buffer
     * @param out Command to be outputted to
     * @returns a TCode Device Command which is a data representation of the device command to be processed easier
     */
    static bool parseDeviceCommand(char *buffer, const size_t length, TCode_Device_Command &out);

    /**
     * @brief Parses a Setup Command
     * @param buffer string to be processed
     * @param length length of buffer
     * @param out a TCode Setup command struct in a data representation to be processed easier
     */
    static bool parseSetupCommand(char *buffer, const size_t length, TCode_Setup_Command &out);

    /**
     * @brief Parses an External Command
     * @param buffer string to be processed
     * @param length length of buffer
     * @param out a TCode Setup command struct in a data representation to be processed easier
     */
    static bool parseExternalCommand(char *buffer, const size_t length, TCode_External_Command &out);
};

#endif