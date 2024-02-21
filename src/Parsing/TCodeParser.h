// TCode-Parser-Class-H v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 26/06/2022
// Please copy, share, learn, innovate, give attribution.
#pragma once
#ifndef TCODE_PARSER_H
#define TCODE_PARSER_H
#include "esp_log.h"
#include "../Constants and Enums/TCodeConstants.h"
#include "../Constants and Enums/TCodeEnums.h"
#include <Arduino.h>

/**
 * @brief Wrapper for parsing commands from TCode
 */
class TCodeParser
{
public:
    /**
     * @brief Converts Lowecase Ascii to Uppercase
     * @param value is the value of the char which needs converting to uppercase
     * @return returns the uppercase representation of the inputted ascii char
     */
    static char toupper(const char value)
    {
        return ((value >= 'a') && (value <= 'z')) ? ('A' + (value - 'a')) : (value);
    }

    /**
     * @brief Checks if the inputed ascii char is a number
     * @param value is the char value which needs checking
     * @return returns true for if the inputted char is a number
     */
    static bool isnumber(const char value)
    {
        switch (value) // Check for numbers
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return true;
        default:
            return false;
        }
    }

    /**
     * @brief Checks if an inputted char is one used for an extention Command in the decoding of TCode
     * @param value is the char value which needs checking
     * @return returns true if the inputted char is used as an extention character
     */
    static bool isextention(const char value)
    {
        switch (toupper(value)) // checks if the inputted char is used as an extention character
        {
        case 'I':
        case 'S':
            return true;
        default:
            return false;
        }
    }

    /**
     * @brief Checks if an inputted char is one used for an ramp Command in the decoding of TCode
     * @param value is the char value which needs checking
     * @return returns true if the inputted char is used as an ramp character
     */
    static bool isramp(const char value)
    {
        switch (toupper(value)) // checks if the inputted char is used as an extention character
        {
        case '<':
        case '>':
        case '=':
            return true;
        default:
            return false;
        }
    }

    /**
     * @brief In a string at the index pointed to by the index paramater gets the Integer contained at and after incrementing the index value
     * @param buffer string to be processed
     * @param length length of buffer
     * @param index the index pointing to a position in the string
     * @return returns a long the integer found in the string represented as base 10 unsigned if a integer string is found which is less than 4 characters long then it will be multiplied til it reaches that minimum e.g. "1" = 1000 ,"01" = 100
     */
    static long getNextInt(unsigned char *buffer, const size_t length, size_t &index)
    {
        size_t count = 0;
        long accum = 0;
        while (isnumber(*(buffer + index))) // while there is a number at the index we are at in the string
        {
            accum *= 10;                                                  // multiply the accumulator first to get the correct output value
            accum += static_cast<long>(toupper(*(buffer + index)) - '0'); // get next int value '0' - '9' subtracting '0' gets the integer value of the next unit
            index++;
            count++; // increase the indeces count to count the digits
        }

        if (count == 0) // no chars were found with 0 - 9
            return -1;

        return accum;
    }

    /**
     * @brief In a string at the index pointed to by the index paramater gets the Integer contained at and after incrementing the index value
     * @param buffer string to be processed
     * @param length length of buffer
     * @param index the index pointing to a position in the string
     * @return returns a long the integer found in the string represented as base 10 unsigned if a integer string is found which is less than 4 characters long then it will be multiplied til it reaches that minimum e.g. "1" = 1000 ,"01" = 100
     */
    static long getNextTCodeInt(unsigned char *buffer, const size_t length, size_t &index)
    {
        size_t count = 0;
        long accum = 0;
        while (isnumber(*(buffer + index))) // while there is a number at the index we are at in the string
        {
            accum *= 10;                                                  // multiply the accumulator first to get the correct output value
            accum += static_cast<long>(toupper(*(buffer + index)) - '0'); // get next int value '0' - '9' subtracting '0' gets the integer value of the next unit
            index++;
            count++; // increase the indeces count to count the digits
        }

        if (count == 0) // no chars were found with 0 - 9
            return -1;

        while (count < 4) // if less than 4 digits were found make up for it by multiplying eg 1 = 1000, 90 = 9000, 001 = 10
        {
            accum *= 10;
            count++;
        }

        return accum;
    }

    /**
     * @brief Gets the char at an index in an inputted buffer
     * @param buffer pointer to buffer to be processed
     * @param length length of buffer
     * @param index index to get char from
     * @return returns a char pointed to at the index position in the buffer (returns '\0' if out of range)
     **/
    static char getCharAt(unsigned char *buffer, const size_t length, size_t index)
    {
        if (index >= length)
            return '\0';
        return *(buffer + index);
    }

    /**
     * @brief Puts a converted int to string into a buffer
     * @param value integer to be converted
     * @param buffer pointer to buffer where string is going to be put
     * @param length length of buffer
     * @param placeIndex index to put the converted int
     * @return returns length of writen value
     **/
    static size_t uintToStr(unsigned long value, unsigned char *buffer, const size_t length, size_t &placeIndex)
    {
        const int base = 10;
        char buf[31];
        int i = 30;

        for(int k = 0; k < 30; k++)
            buf[k] = '\0';


        if (value == 0)
        {
            i = 29;
            buf[29] = '0';
        }

        for (; (value > 0) && (i > 0); --i, value /= base)
        {
            buf[i-1] = "0123456789abcdef"[value % base];
        }

        size_t count = 0;
        for (; i < 30; i++, count++)
        {
            if ((count + placeIndex) > length)
            {
                break;
            }
            buffer[count + placeIndex] = buf[i];
        }
        return count;
    }

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
    static int getStrfromID(unsigned char *buffer, const size_t length, const TCode_ChannelID &id);

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
     * @param startIndex starting index of the char to be processed
     * @returns returns a TCode Axis Extention Type used in the Axis to work out if an extention means Time or Speed
     */
    static TCode_Axis_Extention_Type getExtentionTypeFromStr(unsigned char *buffer, const size_t length, size_t &startIndex);

    /**
     * @brief Gets the Ramp type from a string at the index specified
     * @param buffer string to be processed
     * @param length length of buffer
     * @param startIndex starting index of the char to be processed
     * @returns returns a TCode Ramp Type used in the Axis to work out which ramp to use
     */
    static TCode_Axis_Ramp_Type getRampTypeFromStr(unsigned char *buffer, const size_t length, size_t &startIndex);

    /**
     * @brief Gets the ID from an inputted string from a given index
     * @param buffer string to be processed
     * @param length length of buffer
     * @param startIndex starting index of the char to be processed
     * @returns returns a TCode Channel ID found at the location at the starting index
     */
    static TCode_ChannelID getIDFromStr(unsigned char *buffer, const size_t length, size_t &startIndex);

    /**
     * @brief Returns the type of command provided by the input string
     * @param buffer string to be processed
     * @param length length of buffer
     * @returns a TCode Command Type e.g. Axis, Device, Setup if it is not a valid command None is returned
     */
    static TCode_Command_Type getCommandType(unsigned char *buffer, const size_t length, size_t startIndex);

    /**
     * @brief Parses the next command out of a buffer and puts it into a provided char buffer 
     * @param inputBuffer buffer for command to be parsed from
     * @param buffer buffer for command to go into
     * @param length length of buffer
     * @returns a TCode Command Type e.g. Axis, Device, Setup if it is not a valid command None is returned
     */
    static size_t getNextCommand(TCodeBuffer<char>* inputBuffer,unsigned char *buffer, size_t buffer_length);

    /**
     * @brief Parses an Axis Command
     * @param buffer string to be processed
     * @param length length of buffer
     * @param out a TCode Axis command struct so that the command can be executed easier
     */
    static bool parseAxisCommand(unsigned char *buffer, const size_t length, TCode_Axis_Command &out);

    /**
     * @brief Parses a Device Command
     * @param buffer string to be processed
     * @param length length of buffer
     * @param out Command to be outputted to
     * @returns a TCode Device Command which is a data representation of the device command to be processed easier
     */
    static bool parseDeviceCommand(unsigned char *buffer, const size_t length, TCode_Device_Command &out);

    /**
     * @brief Parses a Setup Command
     * @param buffer string to be processed
     * @param length length of buffer
     * @param out a TCode Setup command struct in a data representation to be processed easier
     */
    static bool parseSetupCommand(unsigned char *buffer, const size_t length, TCode_Setup_Command &out);

    /**
     * @brief Parses an External Command
     * @param buffer string to be processed
     * @param length length of buffer
     * @param out a TCode Setup command struct in a data representation to be processed easier
     */
    static bool parseExternalCommand(unsigned char *buffer, const size_t length, TCode_External_Command &out);
};

#endif