// TCode-C-String-Utils-Class-H v1.0, 
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 29/02/2024
// Please copy, share, learn, innovate, give attribution.
#include "TString.h"

namespace TCode::TString {

/**
 * @brief Checks if an inputted char is one used for an extention Command in the decoding of TCode
 * @param value is the char value which needs checking
 * @return returns true if the inputted char is used as an extention character
 */
bool isextention(const char value)
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
bool isramp(const char value)
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
 * @return returns a long the integer found in the string represented as base 10 unsigned if a integer string is found which is less than 4 characters long then it will be multiplied til it reaches that minimum e.g. "1" = 1000 , "01" = 100
 */
bool getNextInt(unsigned long &value, size_t &log, char *buffer, const size_t length, size_t &index)
{
    size_t count = 0;
    long accum = 0;
    while (isdigit(*(buffer + index))) // while there is a number at the index we are at in the string
    {
        accum *= 10;                                                  // multiply the accumulator first to get the correct output value
        accum += static_cast<long>(toupper(*(buffer + index)) - '0'); // get next int value '0' - '9' subtracting '0' gets the integer value of the next unit
        index++;
        count++; // increase the indeces count to count the digits
    }

    if (count == 0) // no chars were found with 0 - 9
        return false;

    log = count;
    value = accum;

    return true;
}

/**
 * @brief In a string at the index pointed to by the index paramater gets the Integer contained at and after incrementing the index value
 * @param buffer string to be processed
 * @param length length of buffer
 * @param index the index pointing to a position in the string
 * @return returns a long the integer found in the string represented as base 10 unsigned if a integer string is found which is less than 4 characters long then it will be multiplied til it reaches that minimum e.g. "1" = 1000 , "01" = 100
 */
bool getNextTCodeFloat(float &value, size_t &log, char *buffer, const size_t length, size_t &index)
{
    size_t count = 0;
    long accum = 0;
    while (isdigit(*(buffer + index))) // while there is a number at the index we are at in the string
    {
        accum *= 10;                                                  // multiply the accumulator first to get the correct output value
        accum += static_cast<long>(toupper(*(buffer + index)) - '0'); // get next int value '0' - '9' subtracting '0' gets the integer value of the next unit
        index++;
        count++; // increase the indeces count to count the digits
    }

    if (count == 0) // no chars were found with 0 - 9
        return false;

    while (count < 4) // if less than 4 digits were found make up for it by multiplying eg 1 = 1000, 90 = 9000, 001 = 10
    {
        accum *= 10;
        count++;
    }

    value = (double)accum / pow10f((int)count);
    log = count;
    value = accum;

    return true;
}

/**
 * @brief Gets the char at an index in an inputted buffer
 * @param buffer pointer to buffer to be processed
 * @param length length of buffer
 * @param index index to get char from
 * @return returns a char pointed to at the index position in the buffer (returns '\0' if out of range)
 **/
char getCharAt(char *buffer, const size_t length, size_t index)
{
    if (index >= length)
        return '\0';
    return *(buffer + index);
}

/**
 * @brief sets the char at an index in an inputted buffer
 * @param value char to be placed at the index
 * @param buffer pointer to buffer to be processed
 * @param length length of buffer
 * @param index index to get char from
 * @return returns true if the value was written;
 **/
bool setCharAt(char value, char *buffer, const size_t length, size_t index)
{
    if (index >= length)
        return false;
    buffer[index] = value;
    return true;
}

/**
 * @brief Puts a converted int to string into a buffer
 * @param value integer to be converted
 * @param buffer pointer to buffer where string is going to be put
 * @param length length of buffer
 * @param placeIndex index to put the converted int
 * @return returns length of writen value
 **/
size_t uintToStr(unsigned long value, char *buffer, const size_t length, const size_t placeIndex)
{
    const int base = 10;
    char buf[31];
    int i = 30;

    for (int k = 0; k < 30; k++)
        buf[k] = '\0';

    if (value == 0)
    {
        i = 29;
        buf[29] = '0';
    }

    for (; (value > 0) && (i > 0); --i, value /= base)
    {
        buf[i - 1] = "0123456789abcdef"[value % base];
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

size_t uintToStrLen(unsigned long value)
{
    const int base = 10;
    if (value == 0)
    {
        return 1;
    }
    size_t i = 0;
    for (; (value > 0) && (i <= 30); ++i, value /= base) {}
    return i;
}

bool encodeValue(const char *name, bool value, char *buffer, const size_t length)
{
    size_t index = 0;
    size_t nameLength = strlen(name);
    if(setCharAt('#', buffer, length, index++))
        return false;
    for(size_t i = 0; i < nameLength; i++)
    {
        if(setCharAt(name[i], buffer, length, index++))
            return false;
    }

    if(setCharAt(':', buffer, length, index++))
        return false;

    if(value)
    {
        if(setCharAt('1', buffer, length, index++))
            return false;
    }
    else
    {
        if(setCharAt('0', buffer, length, index++))
            return false;
    }
    if(setCharAt('\n', buffer, length, index++))
        return false;
    return true;
}  

bool encodeValue(const char *name, short value, char *buffer, const size_t length)
{
    size_t index = 0;
    size_t nameLength = strlen(name);
    if(setCharAt('$', buffer, length, index++))
        return false;
    for(size_t i = 0; i < nameLength; i++)
    {
        if(setCharAt(name[i], buffer, length, index++))
            return false;
    }

    if(setCharAt(':', buffer, length, index++))
        return false;

    if(value < 0)
    {
        if(setCharAt('-', buffer, length, index++))
            return false;
    }

    unsigned short valueabs = value;
    if(value < 0)
    {
        valueabs = -value;
    }

    index += uintToStr(valueabs, buffer, length, index);
    if(setCharAt('\0', buffer, length, index++))
        return false;
    return true;
}

bool encodeValue(const char *name, unsigned short value, char *buffer, const size_t length)
{
    size_t index = 0;
    size_t nameLength = strlen(name);
    if(setCharAt('@', buffer, length, index++))
        return false;
    for(size_t i = 0; i < nameLength; i++)
    {
        if(setCharAt(name[i], buffer, length, index++))
            return false;
    }

    if(setCharAt(':', buffer, length, index++))
        return false;

    if(value < 0)
    {
        if(setCharAt('-', buffer, length, index++))
            return false;
    }

    index += uintToStr(value, buffer, length, index);
    if(setCharAt('\0', buffer, length, index++))
        return false;
    return true;
}

bool encodeValue(TaggedDataContainer &data, char *buffer, const size_t length)
{
    switch (data.getDataType())
    {
    case VariantType::BOOL:
        {
        bool value;
        if(!data.getValue(value))
        {
            return false;
        }
        encodeValue(data.getTag(), value, buffer, length);
        }
        break;
    case VariantType::INT:
        {
        int value;
        if(!data.getValue(value))
        {
            return false;
        }
        return encodeValue(data.getTag(), (short)value, buffer, length);
        }
        break;
    case VariantType::LONG:
        {
        long value;
        if(!data.getValue(value))
        {
            return false;
        }
        return encodeValue(data.getTag(), (short)value, buffer, length);
        }
    case VariantType::UINT:
        {
        unsigned int value;
        if(!data.getValue(value))
        {
            return false;
        }
        return encodeValue(data.getTag(), (unsigned short)value, buffer, length);
        }
    case VariantType::ULONG:
        {
        unsigned long value;
        if(!data.getValue(value))
        {
            return false;
        }
        return encodeValue(data.getTag(), (unsigned short)value, buffer, length);
        }
    }
    return false;
}


unsigned long getHash(const char *str, size_t length)
{
    //implementation of djb2 from http://www.cse.yorku.ca/~oz/hash.html
    unsigned long hash = 5381;
    char c;
    for(int i = 0, c = str[i]; c != '\0', i < length; i++)
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

}
