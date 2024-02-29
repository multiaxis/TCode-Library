// TCode-Encoder-Class-CPP v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 27/06/2022
// Please copy, share, learn, innovate, give attribution.
#include "TCodeEncoder.h"

bool TCodeEncoder::encodeValue(TCodeTaggedDataContainer &data, char *buffer, const size_t length)
{
    switch (data.getDataType())
    {
    case TCodeInterfaceDataTag::BOOL:
        {
        bool value;
        if(!data.getValue(value))
        {
            return false;
        }
        encodeValue(data.getName(),value,buffer,length);
        }
        break;
    case TCodeInterfaceDataTag::INT:
        {
        int value;
        if(!data.getValue(value))
        {
            return false;
        }
        return encodeValue(data.getName(),(short)value,buffer,length);
        }
        break;
    case TCodeInterfaceDataTag::LONG:
        {
        long value;
        if(!data.getValue(value))
        {
            return false;
        }
        return encodeValue(data.getName(),(short)value,buffer,length);
        }
    case TCodeInterfaceDataTag::UINT:
        {
        unsigned int value;
        if(!data.getValue(value))
        {
            return false;
        }
        return encodeValue(data.getName(),(unsigned short)value,buffer,length);
        }
    case TCodeInterfaceDataTag::ULONG:
        {
        unsigned long value;
        if(!data.getValue(value))
        {
            return false;
        }
        return encodeValue(data.getName(),(unsigned short)value,buffer,length);
        }
    }
    return false;
}

bool TCodeEncoder::encodeValue(const char *name, bool value, char *buffer, const size_t length)
{
    size_t index = 0;
    size_t nameLength = strlen(name);
    if(TCodeCStringUtils::setCharAt('#',buffer,length,index++))
        return false;
    for(size_t i = 0; i < nameLength; i++)
    {
        if(TCodeCStringUtils::setCharAt(name[i],buffer,length,index++))
            return false;
    }

    if(TCodeCStringUtils::setCharAt(':',buffer,length,index++))
        return false;

    if(value)
    {
        if(TCodeCStringUtils::setCharAt('1',buffer,length,index++))
            return false;
    }
    else
    {
        if(TCodeCStringUtils::setCharAt('0',buffer,length,index++))
            return false;
    }
    if(TCodeCStringUtils::setCharAt('\n',buffer,length,index++))
        return false;
    return true;
}  

bool TCodeEncoder::encodeValue(const char *name, short value, char *buffer, const size_t length)
{
    size_t index = 0;
    size_t nameLength = strlen(name);
    if(TCodeCStringUtils::setCharAt('$',buffer,length,index++))
        return false;
    for(size_t i = 0; i < nameLength; i++)
    {
        if(TCodeCStringUtils::setCharAt(name[i],buffer,length,index++))
            return false;
    }

    if(TCodeCStringUtils::setCharAt(':',buffer,length,index++))
        return false;

    if(value < 0)
    {
        if(TCodeCStringUtils::setCharAt('-',buffer,length,index++))
            return false;
    }

    unsigned short valueabs = value;
    if(value < 0)
    {
        valueabs = -value;
    }

    index += TCodeCStringUtils::uintToStr(valueabs,buffer,length,index);
    if(TCodeCStringUtils::setCharAt('\0',buffer,length,index++))
        return false;
    return true;
}

bool TCodeEncoder::encodeValue(const char *name, unsigned short value, char *buffer, const size_t length)
{
    size_t index = 0;
    size_t nameLength = strlen(name);
    if(TCodeCStringUtils::setCharAt('@',buffer,length,index++))
        return false;
    for(size_t i = 0; i < nameLength; i++)
    {
        if(TCodeCStringUtils::setCharAt(name[i],buffer,length,index++))
            return false;
    }

    if(TCodeCStringUtils::setCharAt(':',buffer,length,index++))
        return false;

    if(value < 0)
    {
        if(TCodeCStringUtils::setCharAt('-',buffer,length,index++))
            return false;
    }

    index += TCodeCStringUtils::uintToStr(value,buffer,length,index);
    if(TCodeCStringUtils::setCharAt('\0',buffer,length,index++))
        return false;
    return true;
}
