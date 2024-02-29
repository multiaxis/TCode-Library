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
