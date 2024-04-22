// TCode-C-String-Utils-Class-H v1.0, 
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 29/02/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include "TMath.h"
#include "../settings/TaggedDataContainer.h"

namespace TCode::TString {

/**
 * @brief Checks if an inputted char is one used for an extention Command in the decoding of TCode
 * @param value is the char value which needs checking
 * @return returns true if the inputted char is used as an extention character
 */
bool isextention(const char value);

/**
 * @brief Checks if an inputted char is one used for an ramp Command in the decoding of TCode
 * @param value is the char value which needs checking
 * @return returns true if the inputted char is used as an ramp character
 */
bool isramp(const char value);

/**
 * @brief In a string at the index pointed to by the index paramater gets the Integer contained at and after incrementing the index value
 * @param buffer string to be processed
 * @param length length of buffer
 * @param index the index pointing to a position in the string
 * @return returns a long the integer found in the string represented as base 10 unsigned if a integer string is found which is less than 4 characters long then it will be multiplied til it reaches that minimum e.g. "1" = 1000 , "01" = 100
 */
bool getNextInt(unsigned long &value, size_t &log, char *buffer, const size_t length, size_t &index);

/**
 * @brief In a string at the index pointed to by the index paramater gets the Integer contained at and after incrementing the index value
 * @param buffer string to be processed
 * @param length length of buffer
 * @param index the index pointing to a position in the string
 * @return returns a long the integer found in the string represented as base 10 unsigned if a integer string is found which is less than 4 characters long then it will be multiplied til it reaches that minimum e.g. "1" = 1000 , "01" = 100
 */
bool getNextTCodeFloat(float &value, size_t &log, char *buffer, const size_t length, size_t &index);

/**
 * @brief Gets the char at an index in an inputted buffer
 * @param buffer pointer to buffer to be processed
 * @param length length of buffer
 * @param index index to get char from
 * @return returns a char pointed to at the index position in the buffer (returns '\0' if out of range)
 **/
char getCharAt(char *buffer, const size_t length, size_t index);

/**
 * @brief sets the char at an index in an inputted buffer
 * @param value char to be placed at the index
 * @param buffer pointer to buffer to be processed
 * @param length length of buffer
 * @param index index to get char from
 * @return returns true if the value was written;
 **/
bool setCharAt(char value, char *buffer, const size_t length, size_t index);

/**
 * @brief Puts a converted int to string into a buffer
 * @param value integer to be converted
 * @param buffer pointer to buffer where string is going to be put
 * @param length length of buffer
 * @param placeIndex index to put the converted int
 * @return returns length of writen value
 **/
size_t uintToStr(unsigned long value, char *buffer, const size_t length, const size_t placeIndex);

size_t uintToStrLen(unsigned long value);

bool encodeValue(const char *name, bool value, char *buffer, const size_t length);

bool encodeValue(const char *name, short value, char *buffer, const size_t length);

bool encodeValue(const char *name, unsigned short value, char *buffer, const size_t length);

bool encodeValue(TaggedDataContainer &data, char *buffer, const size_t length);

unsigned long getHash(const char *str, size_t length);

}
