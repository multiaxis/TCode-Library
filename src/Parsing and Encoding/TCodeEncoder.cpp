// TCode-Encoder-Class-CPP v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 27/06/2022
// Please copy, share, learn, innovate, give attribution.
#include "TCodeEncoder.h"

bool TCodeEncoder::encodeValue(const char *name, bool value, char *buffer, const size_t length)
{
    size_t index = 0;
    size_t nameLength = strlen(name);
    if(index+1 > length-1)
        return false;
    buffer[index++] = '#';
    for(size_t i = 0; i < nameLength; i++)
    {
        if(index > length-1)
            return false;
        
        buffer[index++] = name[i];
    }

    if(index+1 > length-1)
        return false;
    buffer[index++] = ':';
    if(index+1 > length-1)
        return false;
    if(value)
    {
        buffer[index++] = '1';
    }
    else
    {
        buffer[index++] = '0';
    }
    if(index+1 > length-1)
        return false;
    buffer[index++] = '\0';
    return true;
}  

bool TCodeEncoder::encodeValue(const char *name, short value, char *buffer, const size_t length)
{
    return false;
}

bool TCodeEncoder::encodeValue(const char *name, unsigned short value, char *buffer, const size_t length)
{
    return false;
}
