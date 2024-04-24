// TCode-C-String-Utils-Class-H v1.0, 
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 29/02/2024
// Please copy, share, learn, innovate, give attribution.
#include "TString.h"

namespace TCode::TString {

bool getNextInt(size_t &index, const char *buffer, const size_t length, unsigned long &value, size_t &log)
{
    size_t startIndex = index;
    long accum = 0;
    while (isdigit(buffer[index]))
    {
        accum *= 10;
        accum += static_cast<long>(toupper(buffer[index]) - '0');
        index++;
    }

    log = index - startIndex;
    if (log == 0)
        return false;

    value = accum;
    return true;
}

bool getNextTCodeFloat(size_t &index, const char *buffer, const size_t length, float &value, size_t &log)
{
    unsigned long valueLong;
    if (!getNextInt(index, buffer, length, valueLong, log))
        return false;

    while (log < 4) {
        valueLong *= 10;
        log++;
    }

    value = (double)valueLong / pow10f((int)log);
    return true;
}

char getCharAtOrDefault(const size_t index, const char *buffer, const size_t length, const char defaultValue)
{
    if (index >= length)
        return defaultValue;
        
    return buffer[index];
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

char axisTypeToChar(const AxisType &type)
{
    switch (type)
    {
    case AxisType::Auxiliary:
        return 'A';
    case AxisType::Linear:
        return 'L';
    case AxisType::Rotation:
        return 'R';
    case AxisType::Vibration:
        return 'V';
    default:
        return '?';
    }
}

String axisIdToString(const AxisId &id)
{
    return axisTypeToChar(id.type) + String((int)id.channel);
}

}
